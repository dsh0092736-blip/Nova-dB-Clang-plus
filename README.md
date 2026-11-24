NovaDB

NovaDB é um banco de dados local baseado em arquivos JSON, inspirado no MongoDB e desenvolvido em C++.

🚀 Recursos

- [x] CreateDatabase()
- [x] GetDatabase()
- [x] CreateCollection()
- [x] GetCollection()
- [x] InsertJson()
- [x] FindJson()
- [x] DeleteJson()
- [x] ClearJson()
- [x] Geração automática de IDs

🛠️ Tecnologias

- C++17
- nlohmann/json
- Filesystem

📦 Instalação

Instale a dependência no Ubuntu:

sudo apt update
sudo apt install nlohmann-json3-dev

Compile o projeto:

g++ main.cpp -o novadb -std=c++17

Execute:

./novadb

💻 Exemplo

NovaDB nova;

nova.CreateDatabase("MeuDatabase");

Database db = nova.GetDatabase("MeuDatabase");

Collection users = db.CreateCollection("Users");

users.InsertJson({
    {"nome", "Neymar"},
    {"idade", 34}
});

json result = users.FindJson("nome", "Neymar");

cout << result.dump(4) << endl;

📁 Armazenamento

O NovaDB utiliza arquivos JSON diretamente no disco. Cada Collection possui seu próprio arquivo.

🗺️ Futuro do projeto

- Integração com Python
- Integração com C#
- Integração com Java
- Integração com JavaScript

📌 Versão

NovaDB v0.1

Projeto experimental em desenvolvimento.

📄 Licença

Este projeto está licenciado sob a MIT License.
