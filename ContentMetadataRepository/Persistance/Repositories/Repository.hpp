#pragma once
#include <pqxx/pqxx>
#include <string>
#include <tuple>
#include <sstream>
#include <boost/uuid/uuid.hpp>

template <typename T, typename... Fields>
class BaseRepository {
public:
    BaseRepository(
        pqxx::work& transaction, 
        const std::string& table_name, 
        const std::tuple<Fields T::*...>& fields)
        : transaction(transaction)
        , table_name(table_name)
        , fields(fields) {}

    void add(const T& entity) {
        std::string sql = generateInsertSQL(entity);
        transaction.exec0(sql);
    }

    void update(const T& entity) {
        std::string sql = generateUpdateSQL(entity);
        transaction.exec0(sql);
    }

    void remove(const boost::uuids::uuid& id) {
        std::string sql = std::format("DELETE FROM {} WHERE id = '{}';", table_name, id);
        transaction.exec0(sql);
    }

    T findById(const boost::uuids::uuid& id) {
        std::string sql = std::format("SELECT * FROM {} WHERE id = '{}';", table_name, id);
        pqxx::result result = transaction.exec(sql);

        if (result.empty()) {
            throw std::runtime_error("Entity not found");
        }

        return mapResultToEntity(result[0]);
    }

    std::vector<T> findAll() {
        std::string sql = std::format("SELECT * FROM {};", table_name);
        pqxx::result result = transaction.exec(sql);

        std::vector<T> entities;
        for (const auto& row : result) {
            entities.push_back(mapResultToEntity(row));
        }

        return entities;
    }

protected:
    pqxx::work& transaction;
    std::string table_name;
    std::tuple<Fields T::*...> fields;

    std::string generateInsertSQL(const T& entity) {
        std::ostringstream columns;
        std::ostringstream values;

        expandTuple(columns, values, entity, fields);

        return std::format("INSERT INTO {} ({}) VALUES ({});", table_name, columns.str(), values.str());
    }

    std::string generateUpdateSQL(const T& entity) {
        std::ostringstream updates;
        expandUpdateTuple(updates, entity, fields);

        return std::format("UPDATE {} SET {} WHERE id = '{}';", table_name, updates.str(), entity.m_uuid);
    }

    virtual T mapResultToEntity(const pqxx::row& row) const = 0;


    template <typename Field, typename... Rest>
    void expandTuple(std::ostringstream& columns, std::ostringstream& values, const T& entity, Field T::* field, Rest... rest) {
        columns << getFieldName(field);
        values << "'" << transaction.esc(entity.*field) << "'"; 
        if constexpr (sizeof...(Rest) > 0) {
            columns << ", ";
            values << ", ";
            expandTuple(columns, values, entity, rest...);
        }
    }

    template <typename Field, typename... Rest>
    void expandUpdateTuple(std::ostringstream& updates, const T& entity, Field T::* field, Rest... rest) {
        updates << getFieldName(field) << " = '" << transaction.esc(entity.*field) << "'";
        if constexpr (sizeof...(Rest) > 0) {
            updates << ", ";
            expandUpdateTuple(updates, entity, rest...);
        }
    }

    template <typename Field>
    std::string getFieldName(Field T::* field) {
        return "field_name";
    }
};