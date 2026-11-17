#pragma once

#include <concepts>
#include <memory>
#include <type_traits>
#include <utility>

template<typename T>
class Ref {
    template<typename U>
    friend class Ref;
    std::shared_ptr<T> ptr_;

    static_assert(!std::is_reference_v<T>);
    static_assert(!std::is_void_v<T>);
    static_assert(!std::is_pointer_v<T>);

    template<typename U>
    static constexpr bool arg_is_ref = false;
    template<typename V>
    static constexpr bool arg_is_ref<Ref<V>> = true;

    template<typename Arg>
    static constexpr bool ref_ctor_arg_ok = !std::same_as<std::decay_t<Arg>, std::nullptr_t>;

public:
    Ref(const Ref&) noexcept = default;
    Ref& operator=(const Ref&) noexcept = default;

    Ref() = delete;
    Ref(Ref&&) = delete;
    Ref& operator=(Ref&&) = delete;
    bool operator!() const = delete;
    explicit operator bool() const = delete;

    template<typename U>
        requires std::convertible_to<U*, T*>
    Ref(const Ref<U>& other) noexcept : ptr_(other.ptr_) {}

    template<typename... Args>
        requires (sizeof...(Args) > 0)
              && ((!arg_is_ref<std::decay_t<Args>>) && ...)
              && ((ref_ctor_arg_ok<Args>) && ...)
              && std::constructible_from<T, Args...>
    explicit Ref(Args&&... args) : ptr_(std::make_shared<T>(std::forward<Args>(args)...)) {}

    [[nodiscard]]
    T& get() const noexcept { return *ptr_; }

    [[nodiscard]]
    T& operator*() const noexcept { return *ptr_; }

    [[nodiscard]]
    T* operator->() const noexcept { return ptr_.get(); }
};