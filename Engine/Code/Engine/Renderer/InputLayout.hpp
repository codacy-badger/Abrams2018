#pragma once

#include "Engine/RHI/RHITypes.hpp"

#include <vector>

class RHIDevice;
struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;

class InputLayout {
public:
    InputLayout(RHIDevice* parent_device);
    ~InputLayout();

    void AddElement(std::size_t memberByteOffset, const ImageFormat& format, const char* semantic, unsigned int inputSlot = 0, bool isVertexData = true, unsigned int instanceDataStepRate = 0);
    void AddElement(const D3D11_INPUT_ELEMENT_DESC& desc);
    void CreateInputLayout(void* byte_code, std::size_t byte_code_length);
    ID3D11InputLayout* GetDxInputLayout() const;
protected:
private:
    std::vector<D3D11_INPUT_ELEMENT_DESC> _elements;
    ID3D11InputLayout* _dx_input_layout = nullptr;
    RHIDevice* _parent_device = nullptr;
};