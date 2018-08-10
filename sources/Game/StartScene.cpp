#include "StartScene.h"

#include <Engine\Utils\time.h>

#include <iostream>

StartScene::StartScene(GraphicsContext* graphicsContext, GraphicsResourceFactory* graphicsResourceFactory, ResourceManager* resourceManager, InputManager* inputManager)
	: Scene(graphicsContext, resourceManager),
	m_inputManager(inputManager),
	m_graphicsResourceFactory(graphicsResourceFactory),
	m_level(nullptr)
{
	loadResources();
	initializeSceneObjects();
}

StartScene::~StartScene()
{
	delete m_level;

	delete m_playerController;
	delete m_player;

	delete m_boxPrimitive;
}

void StartScene::update()
{
	m_playerController->update();

	OBB newPlayerObb = m_player->getWorldPlacedCollider();

	Intersection intersection;
	for (const OBB& obb : m_levelMesh->getColliders()) {
		if (newPlayerObb.intersects(obb, intersection)) {
			m_playerCamera->getTransform()->setPosition(m_playerCamera->getTransform()->getPosition() + intersection.getDirection() * intersection.getDepth());
			m_player->getTransform()->setPosition(m_playerCamera->getTransform()->getPosition());
		}
	}

	Ray downRay(m_playerCamera->getTransform()->getPosition(), vector3(0.0f, -1.0f, 0.0f));
	float distance = 0.0f;

	float minDistance = std::numeric_limits<float>::infinity();
	for (const OBB& obb : m_levelMesh->getColliders()) {
		bool isFloorIntersects = obb.intersects(downRay, distance);

		if (isFloorIntersects) {
			minDistance = std::min(minDistance, distance);
		}
	}

	if (minDistance >= 1.7f) {
		vector3 oldPosition = m_playerCamera->getTransform()->getPosition();

		oldPosition.y -= 0.30f;
		oldPosition.y = std::max(oldPosition.y, 1.7f);

		m_playerCamera->getTransform()->setPosition(oldPosition);
		m_player->getTransform()->setPosition(oldPosition);
	}
}

void StartScene::render()
{
	m_graphicsContext->enableDepthTest();

	m_lightingGpuProgram->bind();
	m_lightingGpuProgram->setParameter("scene.viewTransform", getActiveCamera()->getViewMatrix());
	m_lightingGpuProgram->setParameter("scene.projectionTransform", getActiveCamera()->getProjectionMatrix());

	m_graphicsContext->enableFaceCulling();
	m_graphicsContext->setFaceCullingMode(GraphicsContext::FaceCullingMode::Back);

	// Draw static objects
	m_level->render(m_graphicsContext, m_lightingGpuProgram);

	// Draw animated objects
	m_animatedLightingGpuProgram->bind();
	m_animatedLightingGpuProgram->setParameter("scene.viewTransform", getActiveCamera()->getViewMatrix());
	m_animatedLightingGpuProgram->setParameter("scene.projectionTransform", getActiveCamera()->getProjectionMatrix());

	for (size_t boneIndex = 0; boneIndex < m_playerMesh->getSkeleton()->getBonesCount(); boneIndex++) {
		m_animatedLightingGpuProgram->setParameter("bonesTransforms[" + std::to_string(boneIndex) + "]", m_playerMesh->getSkeleton()->getBones()[boneIndex].getCurrentPoseTransform());

	}

	m_player->render(m_graphicsContext, m_animatedLightingGpuProgram);

	// Draw bounding volumes
	m_graphicsContext->enableWireframeRendering();
	m_graphicsContext->disableFaceCulling();
	m_boundingVolumeGpuProgram->bind();
	m_boundingVolumeGpuProgram->setParameter("scene.viewTransform", getActiveCamera()->getViewMatrix());
	m_boundingVolumeGpuProgram->setParameter("scene.projectionTransform", getActiveCamera()->getProjectionMatrix());

	m_boundingVolumeGpuProgram->setParameter("material.color", vector3(1.0, 1.0, 1.0));

	matrix4 levelTransformationMatrix = m_level->getTransform()->getTransformationMatrix();

	for (const OBB& collider : m_levelMesh->getColliders()) {
		m_boundingVolumeGpuProgram->setParameter("transform.localToWorld", levelTransformationMatrix);

		m_boxPrimitive->setVertices(collider.getVertices());
		m_boxPrimitive->render();
	}

	if (m_isCollision)
		m_boundingVolumeGpuProgram->setParameter("material.color", vector3(0.0, 1.0, 0.0));

	m_boundingVolumeGpuProgram->setParameter("transform.localToWorld", m_player->getTransform()->getTransformationMatrix());
	m_boxPrimitive->setVertices(m_playerMesh->getColliders()[0].getVertices());

	m_graphicsContext->disableWireframeRendering();
}

void StartScene::loadResources()
{
	m_lightingGpuProgram = m_resourceManager->load<GpuProgram>("resources/shaders/phong.fx");
	m_animatedLightingGpuProgram = m_resourceManager->load<GpuProgram>("resources/shaders/phong_animated.fx");

	m_boundingVolumeGpuProgram = m_resourceManager->load<GpuProgram>("resources/shaders/bounding_volume.fx");

	m_levelMesh = m_resourceManager->load<SolidMesh>("resources/models/level.mod", "meshes_level");
	m_playerMesh = m_resourceManager->load<SolidMesh>("resources/models/player/arms.mod", "meshes_player_arms");

	// Player animations
	m_resourceManager->load<Animation>("resources/animations/player/arms_idle.anim", "animations_player_arms_idle");
	m_resourceManager->load<Animation>("resources/animations/player/arms_running.anim", "animations_player_arms_running");
	m_resourceManager->load<Animation>("resources/animations/player/arms_taking.anim", "animations_player_arms_taking");

}

void StartScene::initializeSceneObjects() {
	m_level = new SolidGameObject(m_levelMesh);
	this->registerSceneObject(m_level);

	m_player = new Player(m_playerMesh);
	this->registerSceneObject(m_player);

	m_playerCamera = createCamera("playerCamera");
	setActiveCamera(m_playerCamera);

	m_playerCamera->setNearClipDistance(0.1f);
	m_playerCamera->setFarClipDistance(300.0f);
	m_playerCamera->setFOVy(45.0f);
	m_playerCamera->setAspectRatio((float)m_graphicsContext->getViewportWidth() / m_graphicsContext->getViewportHeight());

	m_playerCamera->getTransform()->fixYAxis();

	m_playerCamera->getTransform()->setPosition(4.27676010, 11.4990520, -7.80581093);
	m_playerCamera->getTransform()->lookAt(0, 0, 10);

	std::vector<Animation*> playerAnimations(PlayerController::PLAYER_STATES_COUNT);
	playerAnimations[(size_t)PlayerController::PlayerState::Idle] = m_resourceManager->getResource<Animation>("animations_player_arms_idle");
	playerAnimations[(size_t)PlayerController::PlayerState::Running] = m_resourceManager->getResource<Animation>("animations_player_arms_running");
	playerAnimations[(size_t)PlayerController::PlayerState::Taking] = m_resourceManager->getResource<Animation>("animations_player_arms_taking");

	m_playerController = new PlayerController(m_player, m_playerCamera, m_inputManager, playerAnimations);
	m_playerController->setMovementSpeed(0.15f);

	initializePrimitives();
}

void StartScene::initializePrimitives()
{
	m_boxPrimitive = new BoxPrimitive(m_graphicsResourceFactory);
}