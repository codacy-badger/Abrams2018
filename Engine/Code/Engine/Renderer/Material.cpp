#include "Engine/Renderer/Material.hpp"

#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"

#include "Engine/Renderer/Renderer.hpp"

#include <algorithm>
#include <filesystem>
#include <sstream>

Material::Material(Renderer* renderer)
    : _renderer(renderer)
    , _textures(CUSTOM_TEXTURE_INDEX_OFFSET, nullptr)
{
    _textures[0] = _renderer->GetTexture("__diffuse");
    _textures[1] = _renderer->GetTexture("__normal");
    _textures[2] = _renderer->GetTexture("__lighting");
    _textures[3] = _renderer->GetTexture("__specular");
    _textures[4] = _renderer->GetTexture("__occlusion");
    _textures[5] = _renderer->GetTexture("__emissive");

    std::size_t count = _renderer->GetMaterialCount();
    std::ostringstream ss;
    ss << '_' << count;
    _name += ss.str();
}

Material::Material(Renderer* renderer, const XMLElement& element)
    : _renderer(renderer)
    , _textures(CUSTOM_TEXTURE_INDEX_OFFSET, nullptr)
{
    _textures[0] = _renderer->GetTexture("__diffuse");
    _textures[1] = _renderer->GetTexture("__normal");
    _textures[2] = _renderer->GetTexture("__lighting");
    _textures[3] = _renderer->GetTexture("__specular");
    _textures[4] = _renderer->GetTexture("__occlusion");
    _textures[5] = _renderer->GetTexture("__emissive");

    std::size_t count = _renderer->GetMaterialCount();
    std::ostringstream ss;
    ss << '_' << count;
    _name += ss.str();

    LoadFromXml(element);
}


bool Material::LoadFromXml(const XMLElement& element) {
    namespace FS = std::filesystem;

    DataUtils::ValidateXmlElement(element, "material", "", "name", "shader,textures");

    _name = DataUtils::ParseXmlAttribute(element, "name", _name);

    auto xml_shader = element.FirstChildElement("shader");
    if(xml_shader != nullptr) {
        DataUtils::ValidateXmlElement(*xml_shader, "shader", "", "src");
        auto file = DataUtils::ParseXmlAttribute(*xml_shader, "src", "");
        FS::path p(file);
        p.make_preferred();
        auto shader = _renderer->GetShader(p.string());
        if(shader == nullptr) {
            std::ostringstream ss;
            ss << "Shader referenced in Material file \"" << _name << "\" does not already exist.";
            ERROR_AND_DIE(ss.str().c_str());
            return false;
        }
        _shader = shader;
    }

    auto xml_textures = element.FirstChildElement("textures");
    if(xml_textures != nullptr) {
        const auto& loaded_textures = _renderer->GetLoadedTextures();
        auto invalid_tex = _renderer->GetTexture("__invalid");

        auto xml_diffuse = xml_textures->FirstChildElement("diffuse");
        if(xml_diffuse) {
            auto file = DataUtils::ParseXmlAttribute(*xml_diffuse, "src", "");
            FS::path p(file);
            p.make_preferred();
            const auto& p_str = p.string();
            bool empty_path = p.empty();
            bool texture_not_exist = !empty_path && loaded_textures.find(p_str) == loaded_textures.end();
            bool invalid_src = empty_path || texture_not_exist;
            auto tex = invalid_src ? invalid_tex : (_renderer->GetTexture(p_str));
            _textures[0] = tex;
        }

        auto xml_normal = xml_textures->FirstChildElement("normal");
        if(xml_normal) {
            auto file = DataUtils::ParseXmlAttribute(*xml_normal, "src", "");
            FS::path p(file);
            p.make_preferred();
            const auto& p_str = p.string();
            bool empty_path = p.empty();
            bool texture_not_exist = !empty_path && loaded_textures.find(p_str) == loaded_textures.end();
            bool invalid_src = empty_path || texture_not_exist;
            auto tex = invalid_src ? invalid_tex : (_renderer->GetTexture(p_str));
            _textures[1] = tex;
        }

        auto xml_lighting = xml_textures->FirstChildElement("lighting");
        if(xml_lighting) {
            auto file = DataUtils::ParseXmlAttribute(*xml_lighting, "src", "");
            FS::path p(file);
            p.make_preferred();
            const auto& p_str = p.string();
            bool empty_path = p.empty();
            bool texture_not_exist = !empty_path && loaded_textures.find(p_str) == loaded_textures.end();
            bool invalid_src = empty_path || texture_not_exist;
            auto tex = invalid_src ? invalid_tex : (_renderer->GetTexture(p_str));
            _textures[2] = tex;
        }

        auto xml_specular = xml_textures->FirstChildElement("specular");
        if(xml_specular) {
            auto file = DataUtils::ParseXmlAttribute(*xml_specular, "src", "");
            FS::path p(file);
            p.make_preferred();
            const auto& p_str = p.string();
            bool empty_path = p.empty();
            bool texture_not_exist = !empty_path && loaded_textures.find(p_str) == loaded_textures.end();
            bool invalid_src = empty_path || texture_not_exist;
            auto tex = invalid_src ? invalid_tex : (_renderer->GetTexture(p_str));
            _textures[3] = tex;
        }

        auto xml_occlusion = xml_textures->FirstChildElement("occlusion");
        if(xml_occlusion) {
            auto file = DataUtils::ParseXmlAttribute(*xml_occlusion, "src", "");
            FS::path p(file);
            p.make_preferred();
            const auto& p_str = p.string();
            bool empty_path = p.empty();
            bool texture_not_exist = !empty_path && loaded_textures.find(p_str) == loaded_textures.end();
            bool invalid_src = empty_path || texture_not_exist;
            auto tex = invalid_src ? invalid_tex : (_renderer->GetTexture(p_str));
            _textures[4] = tex;
        }

        auto xml_emissive = xml_textures->FirstChildElement("emissive");
        if(xml_emissive) {
            auto file = DataUtils::ParseXmlAttribute(*xml_emissive, "src", "");
            FS::path p(file);
            p.make_preferred();
            const auto& p_str = p.string();
            bool empty_path = p.empty();
            bool texture_not_exist = !empty_path && loaded_textures.find(p_str) == loaded_textures.end();
            bool invalid_src = empty_path || texture_not_exist;
            auto tex = invalid_src ? invalid_tex : (_renderer->GetTexture(p_str));
            _textures[5] = tex;
        }

        auto numTextures = DataUtils::GetChildElementCount(*xml_textures, "texture");
        if(numTextures >= MAX_CUSTOM_TEXTURE_COUNT) {
            DebuggerPrintf("Max custom texture count exceeded. Cannot bind more than %i custom textures.", MAX_CUSTOM_TEXTURE_COUNT);
        }
        AddTextureSlots(numTextures);
        for(auto xml_texture = xml_textures->FirstChildElement("texture");
            xml_texture != nullptr;
            xml_texture = xml_texture->NextSiblingElement("texture")) {
            DataUtils::ValidateXmlElement(*xml_texture, "texture", "", "index,src");
            std::size_t index = CUSTOM_TEXTURE_INDEX_OFFSET + DataUtils::ParseXmlAttribute(*xml_texture, std::string("index"), 0u);
            if(index >= CUSTOM_TEXTURE_INDEX_OFFSET + MAX_CUSTOM_TEXTURE_COUNT) {
                continue;
            }
            auto file = DataUtils::ParseXmlAttribute(*xml_texture, "src", "");
            FS::path p(file);
            p.make_preferred();
            const auto& p_str = p.string();
            bool empty_path = p.empty();
            bool texture_not_exist = !empty_path && loaded_textures.find(p_str) == loaded_textures.end();
            bool invalid_src = empty_path || texture_not_exist;
            auto tex = invalid_src ? invalid_tex : (_renderer->GetTexture(p_str));
            _textures[index] = tex;
        }
    }
    return true;
}

void Material::AddTextureSlots(std::size_t count) {
    std::size_t old_size = _textures.size();
    std::size_t new_size = (std::min)(old_size + MAX_CUSTOM_TEXTURE_COUNT, old_size + (std::min)(MAX_CUSTOM_TEXTURE_COUNT, count));
    _textures.resize(new_size);
    for(std::size_t i = old_size; i < new_size; ++i) {
        _textures[i] = nullptr;
    }
}

void Material::AddTextureSlot() {
    AddTextureSlots(1);
}

std::string Material::GetName() const {
    return _name;
}

Shader * Material::GetShader() const {
    return _shader;
}

std::size_t Material::GetTextureCount() const {
    return _textures.size();
}

Texture* Material::GetTexture(std::size_t i) const {
    return _textures[i];
}
