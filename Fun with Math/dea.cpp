#include "stdafx.h"
#include "dea.hpp"

dea::dea(std::unordered_set<wchar_t> sigma, std::unordered_set<size_t> states, size_t init,
	     std::function<size_t(size_t, wchar_t)> delta, std::unordered_set<size_t> accepts)
	: m_sigma(sigma), m_states(states), m_init(init), m_delta(delta), m_accepts(accepts) {
	if (!m_states.count(m_init)) {
		throw std::logic_error("Initial state not in state set.");
	}
}

bool dea::operator()(std::wstring w) const {
	size_t state = m_init;
	for (const auto& a : w) {
		if (!m_sigma.count(a)) { return false; }
		state = m_delta(state, a);
	}
	return m_accepts.count(state);
}