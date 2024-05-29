#pragma once
#include"BaseUser.h"
#include "Admin.h"
#include"User.h"
class CreatorBaseUser
{
public:
	virtual ~CreatorBaseUser() {}
	virtual BaseUser* create_buser() = 0;
	BaseUser* create()
	{
		BaseUser* Buser = this->create_buser();
		return Buser;
	}
};
class CreatorAdmin: public CreatorBaseUser
{
public:
	virtual BaseUser* create_buser() override { return new Admin(); }
};
class CreatorUser : public CreatorBaseUser
{
public:
	virtual BaseUser* create_buser() override { return new User(); }
};

