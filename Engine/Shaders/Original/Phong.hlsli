#pragma pack_matrix( row_major )

static const int MaxLights = 15;

struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular; // Hack: w holds the specular power
};

struct PointLight
{
    Material LightInt;
    float4 Position;
    float4 Attenuation;
    float Range;
};

struct DirectionalLight
{
    Material LightInt;
    float4 Direction;
};

cbuffer LightParameters : register(b5)
{
    int numPointLights;
    PointLight pointlights[MaxLights];
    DirectionalLight directionalLight;
    float4 eyePositionWorld;
};

cbuffer InstanceData : register(b6)
{
    float4x4 inverse;
    Material material;
};

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

float4 Light(float4 position, float4 positionModelSpace, float4 norm)
{
	// Compute light values in model-space
    float4 msEyePos = mul(eyePositionWorld, inverse);
    float4 msDirToEye = normalize(msEyePos - positionModelSpace);

    float4 ambient = float4(0.f, 0.f, 0.f, 0.f);
    float4 diffuse = float4(0.f, 0.f, 0.f, 0.f);
    float4 spec = float4(0.f, 0.f, 0.f, 0.f);

    float4 A, D, S;
    
    ComputeDirectionalLight(material, directionalLight, normalize(norm), msDirToEye, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;
    
    for (int i = 0; i < numPointLights; i++)
    {
        ComputePointLight(material, pointlights[i], positionModelSpace, normalize(norm), msDirToEye, A, D, S);
        ambient += A;
        diffuse += D;
        spec += S;
    }

    return ambient + diffuse + spec;
}