#pragma once

#include <functional>
#include <unordered_set>

class dea {
private:
	const std::unordered_set<wchar_t> m_sigma;
	const std::unordered_set<size_t> m_states;
	std::function<size_t(size_t, wchar_t)> m_delta;
	size_t m_init;
	const std::unordered_set<size_t> m_accepts;

public:
	dea(std::unordered_set<wchar_t>, std::unordered_set<size_t>, size_t,
		std::function<size_t(size_t, wchar_t)>, std::unordered_set<size_t>);

	bool operator()(std::wstring) const;
};