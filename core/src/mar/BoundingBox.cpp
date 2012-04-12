#include "BoundingBox.h"

namespace mar{
    std::ostream &
    BoundingBox::print(std::ostream & os) const {
        os << "[min[" << min << "], max[" << max << "]]";
        return os;
    }
    
    std::ostream& operator<<(std::ostream& os, const BoundingBox& bb) {
        return bb.print(os);
    }
}
