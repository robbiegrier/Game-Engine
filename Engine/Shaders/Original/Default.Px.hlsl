#include "Default.hlsli"

Texture2D mainTexture : register(t0);
SamplerState aSampler : register(s0);

void PhongModel(Material mat, Material lightint, float3 L, float3 normal, float3 DirToEye,
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

void ComputePointLight(Material mat, PointLight pointLightInstance, float4 posms, float4 normal, float4 DirToEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Zeroing out the ADS contributions
    ambient = float4(0, 0, 0, 0);
    diffuse = float4(0, 0, 0, 0);
    spec = float4(0, 0, 0, 0);
    
    
	// we compute the model-space position
    float3 litPosMS = mul(pointLightInstance.Position, inverse).xyz;
    float3 L = litPosMS - posms.xyz;

	// Early out if out of range
    float d = length(L);
    if (d > pointLightInstance.Range)
        return;

    L /= d; // normalize 

    PhongModel(mat, pointLightInstance.LightInt, L, normal.xyz, DirToEye.xyz, ambient, diffuse, spec);

	// Now we attenuate based on range
    float att = 1 / dot(pointLightInstance.Attenuation.xyz, float3(1, d, d * d));
	// Ambient not attenuated
    diffuse *= att;
    spec *= att;
}

float4 main(VS_OUTPUT input) : SV_TARGET
{
	// Compute light values in model-space
    float4 msEyePos = mul(eyePositionWorld, inverse);
    float4 msDirToEye = normalize(msEyePos - input.positionModelSpace);

    float4 ambient = float4(0, 0, 0, 0);
    float4 diffuse = float4(0, 0, 0, 0);
    float4 spec = float4(0, 0, 0, 0);

    float4 A, D, S;

    ComputePointLight(material, pointlight, input.positionModelSpace, normalize(input.norm), msDirToEye, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;

    float4 litColor = ambient + diffuse + spec;
    float4 texSample = mainTexture.Sample(aSampler, input.textureCoordinate);

    float4 litTextureSample = float4(0, 0, 0, 0);
    litTextureSample.x = litColor.x * texSample.x;
    litTextureSample.y = litColor.y * texSample.y;
    litTextureSample.z = litColor.z * texSample.z;
    litTextureSample.w = litColor.w * texSample.w;

    return litTextureSample;
}

