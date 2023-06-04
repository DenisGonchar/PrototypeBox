
#include "PSTraceUtils.h"
#include "DrawDebugHelpers.h"

bool PSTraceUtils::LineTraceSingleByChannel(const UWorld* World, struct FHitResult& OutHit, const FVector& Start, const FVector& End, ECollisionChannel TraceChannel, const FCollisionQueryParams& Params /*= FCollisionQueryParams::DefaultQueryParam*/, const FCollisionResponseParams& ResponseParam /*= FCollisionResponseParams::DefaultResponseParam*/, bool bDrawDebug /*= false*/, float DrawTime /*= -1.0f*/, FColor TraceColor /*= FColor::Black*/)
{
	bool bResult = false;

	bResult = World->LineTraceSingleByChannel(OutHit, Start, End, TraceChannel, Params, ResponseParam);
	bDrawDebug = true;
#if ENABLE_DRAW_DEBUG

	if (bDrawDebug)
	{
		DrawDebugLine(World, Start, End, FColor::Red, false, DrawTime);
		if (!bResult)
		{
			DrawDebugPoint(World, OutHit.ImpactPoint, 10.0f, FColor::Red, false, DrawTime);
		}
	}

#endif
	DrawDebugLine(World, Start, End, FColor::Red, false, DrawTime);
	DrawDebugPoint(World, OutHit.ImpactPoint, 10.0f, FColor::Red, false, DrawTime);

	return bResult;
}
