#include "QuadTree.h"
#include "CollisionManager.h"
#include "Camera.h"

void QuadTree::init(const char* quadtreePath, BaseObject** allObjects)
{
	fstream fs(quadtreePath);
	rootNode.init(fs, allObjects);
}

void QuadTree::removeObjectFromCamera()
{
	int nObject = CAMERA->objectsInCamera.allObjects.size();

	for (int i = 0; i < nObject; i++)
	{
		BaseObject* obj = CAMERA->objectsInCamera.allObjects[i];
		if (!COLLISION->AABBCheck(*CAMERA, *obj))
		{
			CAMERA->objectsInCamera.remove(obj);
			i--;
			nObject--;
		}
	}
}

void QuadTree::fillObjectToCameraFromNode(QuadNode* node)
{
	if (!COLLISION->AABBCheck(*node, *CAMERA))
		return;

	if (node->nNodes != 0)
	{
		fillObjectToCameraFromNode(&node->nodes[0]);
		fillObjectToCameraFromNode(&node->nodes[1]);
		fillObjectToCameraFromNode(&node->nodes[2]);
		fillObjectToCameraFromNode(&node->nodes[3]);
		return;
	}

	if (node->nObjects != 0)
		for (int i = 0; i < node->nObjects; i++)
			CAMERA->objectsInCamera.add(node->objects[i]);
}

void QuadTree::update()
{
	removeObjectFromCamera();
	fillObjectToCameraFromNode(&rootNode);
}

QuadTree::QuadTree(void)
{
}

QuadTree::~QuadTree(void)
{
}