#pragma once

#include "hypergraph.h"
#include "rule.h"

#include <memory>
#include <vector>

namespace cnr
{
class Hyperedge;

std::weak_ptr<Hyperedge> PatternMatch(const AbstractRelation pattern,
                                      const std::vector<std::weak_ptr<Hyperedge>>& edges);
} // namespace cnr
