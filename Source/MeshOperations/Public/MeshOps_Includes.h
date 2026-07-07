#pragma once

#include "Math/Vector.h"
#include "Kismet/KismetMathLibrary.h"

#include "UObject/Object.h"
#include "UObject/ConstructorHelpers.h"

#include "Engine/StaticMeshActor.h"

#include "Components/SceneComponent.h"
#include "Components/ActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"

#include "ProceduralMeshComponent.h"
#include "ProceduralMeshConversion.h"
#include "KismetProceduralMeshLibrary.h"

#include "MeshDescription.h"
#include "StaticMeshDescription.h"
#include "MeshDescriptionBuilder.h"

#include "PhysicsEngine/BodySetup.h"
#include "Rendering/PositionVertexBuffer.h"
#include "RayTracingDefinitions.h" 
#include "RayTracingGeometry.h"
#include "RenderingThread.h"
#include "RenderResource.h"

#include "Exporters/GLTFExporter.h"
#include "Builders/GLTFBuilder.h"
#include "UserData/GLTFMaterialUserData.h"

#if WITH_EDITOR
#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Engine/Blueprint.h"
#include "Engine/SCS_Node.h"
#include "Engine/SimpleConstructionScript.h"
#include "ScopedTransaction.h"
#endif

#define FIELD_PRODUCT TEXT("Product_Name")
#define FIELD_INSTANCE TEXT("Instance_Name")
#define UNNAMED_PRODUCT TEXT("Unnamed_Product")
#define UNNAMED_INSTANCE TEXT("Unnamed_Instance")