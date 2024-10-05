#include <physica/library/utility/identifiable_base.h>

namespace physica::utility
{
IdentifiableBase::IdentifiableBase()
   : uuid_(UUID::Generate())
{
}

IdentifiableBase::~IdentifiableBase()
{
}

bool IdentifiableBase::operator==(const IdentifiableBase& rhs) const
{
   return uuid_ == rhs.getUUID();
}

bool IdentifiableBase::operator!=(const IdentifiableBase& rhs) const
{
   return uuid_ != rhs.getUUID();
}

const UUID IdentifiableBase::getUUID() const
{
   return uuid_;
}
}
