
#pragma pack_matrix( row_major )

Texture2D mainTexture : register(t0);
SamplerState aSampler : register(s0);

struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular; // Hack: w holds the specular power
};

struct PhongADS
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
};

struct PointLight
{
    PhongADS LightInt;
    float4 Position;
    float4 Attenuation;
    float Range;
};

cbuffer AA0 : register(b0)
{
    matrix projectionMatrix;
}

cbuffer AA1 : register(b1)
{
    matrix viewMatrix;
}

cbuffer AA2 : register(b2) // slot 2 (ConstantBufferSlot::World)
{
    matrix worldMatrix;
}

cbuffer AA3 : register(b3) // slot 3 (ConstantBufferSlot::Color)
{
    float4 lightColor;
}

cbuffer AA4 : register(b4) // slot 4 (ConstantBufferSlot::LightPos)
{
    float4 lightPos;
}

cbuffer LightParameters : register(b5)
{
    PointLight pointlight;
    float4 EyePosWorld;
};

cbuffer InstanceData : register(b6)
{
    float4x4 World;
    float4x4 WorldInv;
    Material Mater;
};

void PhongModel(Material mat, PhongADS lightint, float3 L, float3 normal,float3 DirToEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	// every light source adds to the ambient light total
    ambient = mat.Ambient * lightint.Ambient;

    float diffuseFactor = dot(L, normal);

	// Diffuse and specular contribution only if not facing away from light source
    if (diffuseFactor > 0.0f)
    {
        diffuse = diffuseFactor * mat.Diffuse * lightint.Diffuse;

        float3 r = reflect(-L, normal); // Compute the reflected ray of light
        float spec_val = max(dot(r, DirToEye.xyz), 0);
        float specFactor = pow(spec_val, mat.Specular.w);
        spec = specFactor * mat.Specular * lightint.Specular;
    }
    else
    {
        diffuse = float4(0, 0, 0, 0);
        spec = float4(0, 0, 0, 0);
    }
}

void ComputePointLight(Material mat, PointLight PLight, float4 posms, float4 normal, float4 DirToEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Zeroing out the ADS contributions
    ambient = float4(0, 0, 0, 0);
    diffuse = float4(0, 0, 0, 0);
    spec = float4(0, 0, 0, 0);
    
    
	// we compute the model-space position
    float3 litPosMS = mul(PLight.Position, WorldInv).xyz;
    float3 L = litPosMS - posms.xyz;

	// Early out if out of range
    float d = length(L);
    if (d > PLight.Range)
        return;

    L /= d; // normalize 

    PhongModel(mat, PLight.LightInt, L, normal.xyz, DirToEye.xyz, ambient, diffuse, spec);

	// Now we attenuate based on range
    float att = 1 / dot(PLight.Attenuation.xyz, float3(1, d, d * d));
	// Ambient not attenuated
    diffuse *= att;
    spec *= att;
}

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 PosMS : POSITION;
    float2 Tex : TEXCOORD;
    float4 Norm : NORMAL;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
	// Compute light values in model-space
    float4 msEyePos = mul(EyePosWorld, WorldInv);
    float4 msDirToEye = normalize(msEyePos - input.PosMS);

    float4 ambient = float4(0, 0, 0, 0);
    float4 diffuse = float4(0, 0, 0, 0);
    float4 spec = float4(0, 0, 0, 0);

    float4 A, D, S;

    ComputePointLight(Mater, pointlight, input.PosMS, normalize(input.Norm), msDirToEye, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;

    float4 litColor = ambient + diffuse + spec;
    float4 texSample = mainTexture.Sample(aSampler, input.Tex);

    float4 litTextureSample = float4(0, 0, 0, 0);
    litTextureSample.x = litColor.x * texSample.x;
    litTextureSample.y = litColor.y * texSample.y;
    litTextureSample.z = litColor.z * texSample.z;
    litTextureSample.w = litColor.w * texSample.w;

    return litTextureSample;
}


// --- End of File ---
