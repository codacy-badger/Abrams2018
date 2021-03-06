//--------------------------------------------------------------------------------------
// Input/Output Structures
//--------------------------------------------------------------------------------------
// https://msdn.microsoft.com/en-us/library/windows/desktop/ff471566(v=vs.85).asxyp
struct compute_input_t
{
   uint3 group_id : SV_GroupID;              // What is my index within the context of a the dispatch call
   uint  group_idx : SV_GroupIndex;          // Flattend version of the top 
                                             // (z * (width * height) + y * width + x)
   uint3 group_thread_id : SV_GroupThreadID; // My thread within the context of the group
   uint3 dispatch_id : SV_DispatchThreadID;  // My thread within context of the entire dispatch 
                                             // [5x5x1, with threads 5,5,1]  If I was in 2, 2, 0 group 
                                             // and 3, 3, 0 thread, I would be in 13 13 0 thread id
}; 

//--------------------------------------------------------------------------------------
// Textures and Samplers
//--------------------------------------------------------------------------------------

cbuffer CLEAR_CONSTANTS : register(b0)
{
   float4 CLEAR_COLOR;
};


RWTexture2D<unorm float4> tOutput : register(u0);


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
[numthreads(16, 16, 1)]
void Main( compute_input_t input ) 
{
   tOutput[input.dispatch_id.xy] = CLEAR_COLOR;
}
