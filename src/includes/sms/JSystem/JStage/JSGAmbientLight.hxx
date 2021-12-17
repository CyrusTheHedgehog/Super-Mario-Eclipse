#pragma once

#include "JSGObject.hxx"
#include "types.h"

namespace JStage {

class TAmbientLight : public TObject {
public:
  virtual ~TAmbientLight();

  virtual u32 JSGFGetType() const;
};

} // namespace JStage