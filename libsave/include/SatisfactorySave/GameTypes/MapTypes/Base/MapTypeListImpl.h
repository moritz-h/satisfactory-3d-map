#pragma once

#include <vector>

#include "MapTypeList.h"
#include "MapTypeListVisitor.h"

namespace SatisfactorySave {

    template<typename Impl, typename T>
    class MapTypeListImplBase : public MapTypeList {
    public:
        std::unique_ptr<MapTypeList> clone() override {
            return std::make_unique<Impl>(*dynamic_cast<Impl*>(this));
        };

        void accept(MapTypeListVisitor& v) override {
            v.visit(static_cast<Impl&>(*this));
        }

        [[nodiscard]] std::size_t size() const override {
            return List.size();
        }

        void resize(std::size_t s) override {
            List.resize(s);
        }

        std::vector<T> List;
    };

    template<typename Impl, typename T>
    class MapTypeListImplBase<Impl, std::unique_ptr<T>> : public MapTypeList {
    public:
        MapTypeListImplBase() = default;

        MapTypeListImplBase(const MapTypeListImplBase& other) : MapTypeList(other) {
            List.reserve(other.List.size());
            for (const auto& l : other.List) {
                List.push_back(std::move(l->clone()));
            }
        }

        MapTypeListImplBase& operator=(const MapTypeListImplBase& other) {
            if (this != &other) {
                MapTypeList::operator=(other);
                List.clear();
                List.reserve(other.List.size());
                for (const auto& l : other.List) {
                    List.push_back(std::move(l->clone()));
                }
            }
            return *this;
        }

        MapTypeListImplBase(MapTypeListImplBase&&) = default;
        MapTypeListImplBase& operator=(MapTypeListImplBase&&) = default;

        std::unique_ptr<MapTypeList> clone() override {
            return std::make_unique<Impl>(*dynamic_cast<Impl*>(this));
        };

        void accept(MapTypeListVisitor& v) override {
            v.visit(static_cast<Impl&>(*this));
        }

        [[nodiscard]] std::size_t size() const override {
            return List.size();
        }

        void resize(std::size_t s) override {
            List.resize(s);
        }

        std::vector<std::unique_ptr<T>> List;
    };

    template<typename Impl, typename T>
    class MapTypeListImpl : public MapTypeListImplBase<Impl, T> {
    public:
        void serializeEntry(Archive& ar, std::size_t i) override {
            if (this->List.size() <= i) {
                this->List.resize(i + 1);
            }
            ar << this->List[i];
        }
    };
} // namespace SatisfactorySave
