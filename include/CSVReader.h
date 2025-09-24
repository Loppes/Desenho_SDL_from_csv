#pragma once
#include "Objetos.h"
#include <string>
#include <vector>
#include <memory>

void lerCSV(const std::string& arquivo, Tela& tela, std::vector<std::shared_ptr<Objeto>>& objetos);