#pragma once
class BaseObject;

class OldRestore
{
public:
	int oldX, oldY;

	virtual void getFromObject(BaseObject* obj);
	virtual void restore(BaseObject* obj);

	OldRestore();
	~OldRestore();
};