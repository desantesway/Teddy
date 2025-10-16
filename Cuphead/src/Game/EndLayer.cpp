#include "EndLayer.h"

#include "Teddy/PostProcessing/PostProcessing.h"

EndLayer::EndLayer()
	: Layer("Ending Layer")
{
	TED_PROFILE_FUNCTION();
}

void EndLayer::OnUpdate(Teddy::Timestep& ts)
{
	TED_PROFILE_FUNCTION();

    {
        TED_PROFILE_SCOPE("Post Processing");

        Teddy::PostProcessing::Unbind();

        Teddy::PostProcessing::Apply();
    }
}

