
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include <algorithm>

using namespace std;
namespace fs = filesystem;
using json = nlohmann::json;

class Collection {
public:
    string name;
    fs::path path;

    Collection(string collectionName, fs::path collectionPath) {
        name = collectionName;
        path = collectionPath;
    }

    
        void InsertJson(json document) {
    if (!document.is_object()) {
        throw runtime_error("Documento precisa ser um objeto JSON!");
    }

    ifstream input(path);

    if (!input.is_open()) {
        throw runtime_error("Erro ao abrir Collection!");
    }

    json documents;
    input >> documents;
    input.close();

    if (!documents.is_array()) {
        throw runtime_error("Collection invalida!");
    }

    // Gerar ID automaticamente
    int nextId = 1;

    for (const auto& item : documents) {
        if (item.contains("id") && item["id"].is_number_integer()) {
            int currentId = item["id"].get<int>();

            if (currentId >= nextId) {
                nextId = currentId + 1;
            }
        }
    }

    document["id"] = nextId;

    documents.push_back(document);

    ofstream output(path);

    if (!output.is_open()) {
        throw runtime_error("Erro ao salvar documento!");
    }

    output << documents.dump(4);

    cout << "Documento inserido! ID: " << nextId << "\n";
        }

void DeleteJson(int id) {

    ifstream input(path);

    if (!input.is_open()) {
        throw runtime_error("Erro ao abrir Collection!");
    }

    json documents;
    input >> documents;
    input.close();

    auto oldSize = documents.size();

    documents.erase(
        remove_if(
            documents.begin(),
            documents.end(),
            [id](const json& document) {
                return document.contains("id") &&
                       document["id"].is_number_integer() &&
                       document["id"].get<int>() == id;
            }
        ),
        documents.end()
    );

    if (documents.size() == oldSize) {
        cout << "Documento nao encontrado!\n";
        return;
    }

    ofstream output(path);

    if (!output.is_open()) {
        throw runtime_error("Erro ao salvar Collection!");
    }

    output << documents.dump(4);

    cout << "Documento excluido!\n";
}

void ClearJson() {

    ofstream output(path);

    if (!output.is_open()) {
        throw runtime_error("Erro ao limpar Collection!");
    }

    output << "[]";

    cout << "Collection limpa!\n";
}
json FindJson(const string& key, const json& value) {
    ifstream input(path);

    if (!input.is_open()) {
        throw runtime_error("Erro ao abrir Collection!");
    }

    json documents;
    input >> documents;
    input.close();

    if (!documents.is_array()) {
        throw runtime_error("Collection invalida!");
    }

    json results = json::array();

    for (const auto& document : documents) {
        if (document.is_object() &&
            document.contains(key) &&
            document[key] == value) {

            results.push_back(document);
        }
    }

    return results;
}

};

class Database {
public:
    string name;
    int version;

    Database(string dbName, int dbVersion) {
        name = dbName;
        version = dbVersion;
    }

    Collection CreateCollection(string collectionName) {

    fs::path collectionPath =
        fs::path("NovaDB/Databases") / name /
        (collectionName + ".json");

    if (!fs::exists(collectionPath)) {
        ofstream file(collectionPath);

        if (!file.is_open()) {
            throw runtime_error("Erro ao criar Collection!");
        }

        file << "[]";
    }

    return Collection(collectionName, collectionPath);
    }
};

class NovaDB {
public:

    void CreateDatabase(string name) {

        fs::path path = fs::path("NovaDB/Databases") / name;

        if (fs::exists(path)) {
            cout << "Database ja existe!\n";
            return;
        }

        fs::create_directories(path);

        json metadata = {
            {"name", name},
            {"version", 1}
        };

        ofstream file(path / "database.json");
        file << metadata.dump(4);
        file.close();

        cout << "Database criado!\n";
    }

    Database GetDatabase(string name) {

        fs::path path =
            fs::path("NovaDB/Databases") / name / "database.json";

        if (!fs::exists(path)) {
            throw runtime_error("Database nao encontrado!");
        }

        ifstream file(path);

        json metadata;
        file >> metadata;

        return Database(
            metadata.at("name").get<string>(),
            metadata.at("version").get<int>()
        );
    }
};

int main() {

    NovaDB nova;

    Database database = nova.GetDatabase("MeuDatabase");

    Collection users = database.CreateCollection("Users");

    users.InsertJson({
        {"nome", "Neymar"}
    });

    users.InsertJson({
        {"nome", "Yamal"}
    });

    json results = users.FindJson("nome", "Yamal");

cout << results.dump(4) << endl;

    return 0;
}