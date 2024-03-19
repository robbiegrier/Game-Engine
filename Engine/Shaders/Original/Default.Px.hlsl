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
    if (diffuseFactor > 0.f)
    {
        diffuse = diffuseFactor * mat.Diffuse * lightint.Diffuse;

        float3 r = reflect(-L, normal); // Compute the reflected ray of light
        float spec_val = max(dot(r, DirToEye.xyz), 0.f);
        float specFactor = pow(spec_val, mat.Specular.w);
        spec = specFactor * mat.Specular * lightint.Specular;
    }
    else
    {
        diffuse = float4(0.f, 0.f, 0.f, 0.f);
        spec = float4(0.f, 0.f, 0.f, 0.f);
    }
}

void ComputePointLight(Material mat, PointLight pointLightInstance, float4 posms, float4 normal, float4 DirToEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
    ambient = float4(0.f, 0.f, 0.f, 0.f);
    diffuse = float4(0.f, 0.f, 0.f, 0.f);
    spec = float4(0.f, 0.f, 0.f, 0.f);
    
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
    
    // MOD: Adjusted by approx Scale
    float approxScale = length(inverse._m00_m01_m02);
    att *= approxScale;
    
	// Ambient not attenuated
    diffuse *= att;
    spec *= att;
}

void ComputeDirectionalLight(Material mat, DirectionalLight lightInstance, float4 normal, float4 DirToEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
    ambient = float4(0.f, 0.f, 0.f, 0.f);
    diffuse = float4(0.f, 0.f, 0.f, 0.f);
    spec = float4(0.f, 0.f, 0.f, 0.f);

	// we compute ligth in the model-space
	// For directional light, the source is infinitely far, so translation have no effect
    float3 L = normalize(mul(-lightInstance.Direction.xyz, (float3x3) inverse));

    PhongModel(mat, lightInstance.LightInt, L, normal.xyz, DirToEye.xyz, ambient, diffuse, spec);
}

float4 main(VS_OUTPUT input) : SV_TARGET
{
	// Compute light values in model-space
    float4 msEyePos = mul(eyePositionWorld, inverse);
    float4 msDirToEye = normalize(msEyePos - input.positionModelSpace);

    float4 ambient = float4(0.f, 0.f, 0.f, 0.f);
    float4 diffuse = float4(0.f, 0.f, 0.f, 0.f);
    float4 spec = float4(0.f, 0.f, 0.f, 0.f);

    float4 A, D, S;
    
    ComputeDirectionalLight(material, directionalLight, normalize(input.norm), msDirToEye, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;
    
    for (int i = 0; i < numPointLights; i++)
    {
        ComputePointLight(material, pointlights[i], input.positionModelSpace, normalize(input.norm), msDirToEye, A, D, S);
        ambient += A;
        diffuse += D;
        spec += S;
    }

    float4 litColor = ambient + diffuse + spec;
    

    float4 texSample = mainTexture.Sample(aSampler, input.textureCoordinate);
    float4 litTextureSample = litColor * texSample;
    
    return litTextureSample;
}

