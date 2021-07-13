#include "sms/spc/SpcBinary.hxx"

#include "sme/sunscript/common.hxx"
#include "SME.hxx"

using namespace SME;

#define BIND_SYMBOL(binary, symbol, func) (binary)->bindSystemDataToSymbol((symbol), reinterpret_cast<u32>(&(func)))
void Patch::Spc::initCustomFunctions(TSpcBinary *spcBinary, const char *symbol, u32 address) {
    spcBinary->bindSystemDataToSymbol(symbol, address);
    BIND_SYMBOL(spcBinary, "setActorPosToOther", Util::Spc::setActorPosToOther);
    BIND_SYMBOL(spcBinary, "setActorRotToOther", Util::Spc::setActorRotToOther);
    BIND_SYMBOL(spcBinary, "getActorPos", Util::Spc::getActorPos);
    BIND_SYMBOL(spcBinary, "setActorPos", Util::Spc::setActorPos);
    BIND_SYMBOL(spcBinary, "getActorRot", Util::Spc::getActorRot);
    BIND_SYMBOL(spcBinary, "setActorRot", Util::Spc::setActorRot);
    BIND_SYMBOL(spcBinary, "spawnObjByID", Util::Spc::spawnObjByID);
    BIND_SYMBOL(spcBinary, "isMultiplayerActive", Util::Spc::isMultiplayerActive);
    BIND_SYMBOL(spcBinary, "isFreePlayActive", Util::Spc::isFreePlayActive);
    BIND_SYMBOL(spcBinary, "isCompletionRewardActive", Util::Spc::isCompletionRewardActive);
    BIND_SYMBOL(spcBinary, "getActivePlayers", Util::Spc::getActivePlayers);
    BIND_SYMBOL(spcBinary, "getMaxPlayers", Util::Spc::getMaxPlayers);
    BIND_SYMBOL(spcBinary, "getPlayerByIndex", Util::Spc::getPlayerByIndex);
    BIND_SYMBOL(spcBinary, "getDateAsStr", Util::Spc::getDateAsStr);
    BIND_SYMBOL(spcBinary, "getTimeAsStr", Util::Spc::getTimeAsStr);
    BIND_SYMBOL(spcBinary, "getPlayerInputByIndex", Util::Spc::getPlayerInputByIndex);
    BIND_SYMBOL(spcBinary, "read8", Util::Spc::read8);
    BIND_SYMBOL(spcBinary, "read16", Util::Spc::read16);
    BIND_SYMBOL(spcBinary, "read32", Util::Spc::read32);
    BIND_SYMBOL(spcBinary, "write8", Util::Spc::write8);
    BIND_SYMBOL(spcBinary, "write16", Util::Spc::write16);
    BIND_SYMBOL(spcBinary, "write32", Util::Spc::write32);
    BIND_SYMBOL(spcBinary, "memcpy", Util::Spc::memcpy_);
    BIND_SYMBOL(spcBinary, "memmove", Util::Spc::memmove_);
    BIND_SYMBOL(spcBinary, "memcmp", Util::Spc::memcmp_);
    BIND_SYMBOL(spcBinary, "memset", Util::Spc::memset_);
    BIND_SYMBOL(spcBinary, "formatStrBySpec", Util::Spc::formatStrBySpec);
}
#undef BIND_SYMBOL