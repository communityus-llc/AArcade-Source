#ifndef C_INSTANCE_MANAGER_H
#define C_INSTANCE_MANAGER_H

#include "KeyValues.h"
#include <map>
#include <vector>
#include <string>
#include "c_prop_shortcut.h"

struct object_t
{
	std::string objectId;
	unsigned int created;
	std::string creator;
	unsigned int removed;
	std::string remover;
	unsigned int modified;
	std::string modifier;
	std::string itemId;
	std::string modelId;
	Vector origin;
	QAngle angles;
	bool spawned;
	float scale;
	bool slave;
};

struct instance_t
{
	std::string id;
	std::string mapId;
	std::string title;
	std::string file;
	std::string workshopIds;
	std::string mountIds;
};

class C_InstanceManager
{
public:
	C_InstanceManager();
	~C_InstanceManager();

	void LevelShutdownPostEntity();

	void Update();

	void LoadInstance(std::string instanceId);
	void LoadLegacyInstance(std::string instanceId);

	void ApplyChanges(std::string id, C_PropShortcutEntity* pShortcut);
	void ResetObjectChanges(C_PropShortcutEntity* pShortcut);

	void SpawnObject(object_t* object, bool bShouldGhost = false);
	object_t* AddObject(std::string objectId, std::string itemId, std::string modelId, Vector origin, QAngle angles, float scale, bool slave, unsigned int created = 0, std::string creator = "", unsigned int removed = 0, std::string remover = "", unsigned int modified = 0, std::string modifier = "");
	object_t* GetInstanceObject(std::string objectId);
	void RemoveEntity(C_PropShortcutEntity* pShortcutEntity);
	bool SpawnNearestObject();
	void SetNearestSpawnDist(double maxDist) { m_fNearestSpawnDist = (float)m_fNearestSpawnDist = maxDist; }

	void AddInstance(std::string instanceId, std::string mapId, std::string title, std::string file, std::string workshopIds, std::string mountIds);
	instance_t* GetInstance(std::string id);
	instance_t* FindInstance(std::string mapId);
	void FindAllInstances(std::string mapId, std::vector<instance_t*> &instances);
	void LegacyMapIdFix(std::string legacyMapName, std::string mapId);

	void SpawnActionPressed();
	void ChangeModel(C_BaseEntity* pEntity, std::string modelId, std::string in_modelFile);
	//void SpawnItem(std::string id);

	// accessors

	// mutators
	
private:
	KeyValues* m_pInstanceKV;
	unsigned int m_uNextFlashedObject;
	float m_fLastSpawnActionPressed;
	float m_fNearestSpawnDist;
	std::map<std::string, object_t*> m_objects;
	std::vector<object_t*> m_unspawnedObjects;

	std::map<std::string, instance_t*> m_instances;
};

#endif