#pragma once
#include <utility>
#include <vector>
#include <string>
#include <optional>
#include <algorithm>
#include <functional>
#include <typeinfo>
#include <typeindex>

using type_id = std::type_index;


class Sup {
	Sup *_parent;
	std::vector<Sup *> _children;
	std::vector<std::function<void()>> _managed;

public:
	virtual std::optional<std::string> name() const {return std::nullopt;}
	Sup(Sup *parent = nullptr);
	virtual ~Sup();

	virtual bool isA(type_id const &what) const;

	void manage(std::function<void()> freeCb);

	void addChild(Sup *);
	void remove(Sup *);

	inline Sup *parent() const {return _parent;};

	[[nodiscard]] auto children(type_id const &t) const -> std::vector<Sup*> { return children(t, false); }
	[[nodiscard]] auto children(type_id const &t, bool recursive) const -> std::vector<Sup*>;
	[[nodiscard]] auto findChild(type_id const &t) const -> Sup* { return findChild(t, std::nullopt, true); }
	[[nodiscard]] auto findChild(type_id const &t, std::optional<std::string> name) const -> Sup* { return findChild(t, std::move(name), true); }
	[[nodiscard]] auto findChild(type_id const &t, std::optional<std::string> name, bool recursive) const -> Sup*;
	[[nodiscard]] auto findParent(type_id const &t) const -> Sup* { return findParent(t, std::nullopt); }
	[[nodiscard]] auto findParent(type_id const &t, std::optional<std::string> name) const -> Sup*;
	[[nodiscard]] auto children(std::type_info const &t, bool recursive = false) const -> std::vector<Sup*> { return children(type_id(t), recursive); }
	[[nodiscard]] auto findChild(std::type_info const &t, std::optional<std::string> name = std::nullopt, bool recursive = true) const -> Sup* { return findChild(type_id(t), std::move(name), recursive); }
	[[nodiscard]] auto findParent(std::type_info const &t, std::optional<std::string> name = std::nullopt) const -> Sup* { return findParent(type_id(t), std::move(name)); }

	template <typename T = Sup>
	auto children(const bool recursive = false) const -> std::vector<T*> {
		auto ch = children(typeid(T), recursive);
		std::vector<T*> res;
		for (auto &c : ch)
			if (T* casted = dynamic_cast<T*>(c))
				res.push_back(casted);
		return res;
	}

	template <typename T>
	auto findChild(std::optional<std::string> name = std::nullopt, const bool recursive = true) const -> T* {
		auto c = findChild(typeid(T), std::move(name), recursive);
		return dynamic_cast<T*>(c);
	}

	template <typename T>
	auto findParent(std::optional<std::string> name = std::nullopt) const -> T* {
		auto p = findParent(typeid(T), std::move(name));
		return dynamic_cast<T*>(p);
	}
};
