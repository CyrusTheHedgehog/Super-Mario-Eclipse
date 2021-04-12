#include "CheatHandler.hxx"

using namespace SME;

Class::TCheatHandler::TCheatHandler() {
  this->mInputList = nullptr;
  this->mSuccessCallback = nullptr;
  this->mFailureCallback = nullptr;
  this->mGamePad = nullptr;
  this->mActive = false;
  this->mInputIndex = 0;
}

Class::TCheatHandler::TCheatHandler(u16 *inputList, TMarioGamePad *gamepad,
                                    void (*successCallback)(TCheatHandler *),
                                    void (*failureCallback)(TCheatHandler *)) {
  this->mInputList = inputList;
  this->mSuccessCallback = successCallback;
  this->mFailureCallback = failureCallback;
  this->mGamePad = gamepad;
  this->mActive = false;
  this->mInputIndex = 0;
}

void Class::TCheatHandler::advanceInput() {
  if (!this->mGamePad || !this->mInputList)
    return;
  else if (this->mGamePad->mButtons.mFrameInput == 0 ||
           this->mInputIndex < 0)
    return;

  if (this->mGamePad->mButtons.mFrameInput !=
      this->mInputList[static_cast<u8>(this->mInputIndex)]) {
    this->mInputIndex = -1;
    this->mActive = false;

    if (this->mFailureCallback)
      this->mFailureCallback(this);

    return;
  }

  if (this->mInputList[static_cast<u8>(this->mInputIndex) + 1] == 0) {
    this->mInputIndex = -1;
    this->mActive = true;

    if (this->mSuccessCallback)
      this->mSuccessCallback(this);

    return;
  }

  this->mInputIndex += 1;
}

void Class::TCheatHandler::reset() {
  this->mInputIndex = 0;
  this->mActive = false;
}