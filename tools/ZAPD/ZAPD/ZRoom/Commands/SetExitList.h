#pragma once

#include "ZRoom/ZRoomCommand.h"

class SetExitList : public ZRoomCommand
{
public:
	SetExitList(ZFile* nParent);

	void DeclareReferences(const std::string& prefix) override;
	void ParseRawDataLate() override;
	void DeclareReferencesLate(const std::string& prefix) override;

	std::string GetBodySourceCode() const override;

	RoomCommand GetRoomCommand() const override;
	std::string GetCommandCName() const override;

private:
	std::vector<uint16_t> exits;
};
