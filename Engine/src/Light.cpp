#include "Light.h"
#include "LightSystem.h"
#include "imgui.h"
#include "GOConstColor.h"

namespace Azul
{
	void Light::Update(float deltaTime)
	{
		static_cast<void>(deltaTime);

		float attenuationValue = 1.f / (intensity);
		attenuation.set(attenuationValue, attenuationValue, attenuationValue, 1.f);

		switch (lightType)
		{
		case Azul::Light::Type::Directional:
			PushDirectionalLight();
			break;
		case Azul::Light::Type::Point:
			PushPointLight();
			break;
		default:
			break;
		}

		GetGameObject()->SetAlwaysRenderShell(true);
		((GOConstColor*)GetGameObject()->GetBoundingBoxVisual())->SetColor(Vec3(lightMaterial.diffuse));
	}

	void Light::PushDirectionalLight()
	{
		DirectionalLight data;
		data.light = lightMaterial;
		data.direction = GetGameObject()->GetWorld().get(Row4::i2).getNorm();
		LightSystem::SetDirectionalLight(data);
	}

	void Light::PushPointLight()
	{
		PointLight data;
		data.light = lightMaterial;
		data.attenuation = attenuation;
		data.range = range;
		data.position = Vec4(GetGameObject()->GetWorldLocation(), 1.f);
		LightSystem::SetPointLight(data);
	}

	void Light::Serialize(json& out)
	{
		out["Type"] = "Light";
		out["Light Type"] = lightType;
		out["Ambient"] = { lightMaterial.ambient[x], lightMaterial.ambient[y], lightMaterial.ambient[z], lightMaterial.ambient[w] };
		out["Diffuse"] = { lightMaterial.diffuse[x], lightMaterial.diffuse[y], lightMaterial.diffuse[z], lightMaterial.diffuse[w] };
		out["Specular"] = { lightMaterial.specular[x], lightMaterial.specular[y], lightMaterial.specular[z], lightMaterial.specular[w] };
		out["Attenuation"] = { attenuation[x], attenuation[y], attenuation[z], attenuation[w] };
		out["Range"] = range;
	}

	void Light::Deserialize(const json& in)
	{
		lightType = in["Light Type"];

		const auto& la = in["Ambient"].template get<std::vector<float>>();
		lightMaterial.ambient.set(la[0], la[1], la[2], la[3]);

		const auto& ld = in["Diffuse"].template get<std::vector<float>>();
		lightMaterial.diffuse.set(ld[0], ld[1], ld[2], ld[3]);

		const auto& ls = in["Specular"].template get<std::vector<float>>();
		lightMaterial.specular.set(ls[0], ls[1], ls[2], ls[3]);

		const auto& lat = in["Attenuation"].template get<std::vector<float>>();
		attenuation.set(lat[0], lat[1], lat[2], lat[3]);

		range = in["Range"];

		intensity = 1.f / (attenuation[x]);
	}

	void Light::UpdateInspectorGui()
	{
		ImGuiTreeNodeFlags flags;
		flags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen;

		ImGui::Spacing();

		if (ImGui::CollapsingHeader("Light", flags))
		{
			ImGui::DragFloat("Intensity", &intensity);
			ImGui::ColorPicker3("Color Diffuse", &lightMaterial.diffuse[x]);
			ImGui::DragFloat("Range", &range);

			if (ImGui::CollapsingHeader("Details"))
			{
				ImGui::ColorPicker3("Ambient", &lightMaterial.ambient[x]);
				ImGui::ColorPicker3("Specular", &lightMaterial.specular[x]);
			}
		}
	}
}