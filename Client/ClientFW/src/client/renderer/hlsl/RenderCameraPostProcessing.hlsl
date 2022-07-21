#include "Resource.hlsl"

Texture2D g_input : register(t0, space0);
RWTexture2D<float4> g_output :register(u0, space0);

[numthreads(16, 16, 1)]
void CSSobelEdge(int3 dispatch_thread_id : SV_DispatchThreadID)
{
    float4 neighbor[3][3];
    for (int y = 0; y < 3; ++y)
    {
        for (int x = 0; x < 3; ++x)
        {
            int2 xy = dispatch_thread_id.xy + int2(-1 + y, -1 + x);
            neighbor[y][x] = g_input[xy];
        }
    }
    
    float4 gx = -1.0f * neighbor[0][0] - 2.0f * neighbor[1][0] - 1.0f * neighbor[2][0] +
    1.0f * neighbor[0][2] + 2.0f * neighbor[1][2] + 1.0f * neighbor[2][2];
    
    float4 gy = -1.0f * neighbor[2][0] - 2.0f * neighbor[2][1] - 1.0f * neighbor[2][2] +
    1.0f * neighbor[0][0] + 2.0f * neighbor[0][1] + 1.0f * neighbor[0][2];

    float4 mag = sqrt(gx * gx + gy * gy);
    mag = 1.0f - saturate(dot(mag.rgb, float3(0.299f, 0.587f, 0.114f)));
    
    g_output[dispatch_thread_id.xy] = g_input[dispatch_thread_id.xy] * mag;
}