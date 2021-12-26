/**
 * This file is part of Open Chess Game Database Standard.
 *
 * Copyright (c) 2021 Nguyen Pham (github@nguyenpham)
 * Copyright (c) 2021 Developers
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include "builder.h"
#include "board/chess.h"

int main(int argc, const char * argv[]) {
    std::cout << "Open Chess Game Database Standard, Database Builder, (C) 2021\n";
    
    // init
    {
        bslib::ChessBoard::staticInit();
    }

    ocgdb::Builder oc;
    
    
//    {
//        std::cout << "Testing..." << std::endl;
//        std::unordered_map<uint64_t, ocgdb::HashData*> hashMap;
//
//        int64_t cnt = 0;
//        for(int i = 0; i < 300 * 1024 * 1024; i++) {
//            auto r = new ocgdb::HashData;
//            for(int j = 0; j < 40; j++) {
//                r->gameIdVec.push_back(j);
//                cnt++;
//            }
//            hashMap[i] = r;
//        }
//        std::cout << "Tested. DONE! cnt: " << cnt << std::endl;
//    }

    auto benchMode = 0, cpu = 0;
    std::string pgnPath, dbPath;
    for(auto i = 1; i < argc; i++) {
        auto str = std::string(argv[i]);
        if (str == "-bench") {
            benchMode = 1;
            continue;
        }
        if (str == "-benchmoves") {
            benchMode = 2;
            continue;
        }

        if (i + 1 >= argc) continue;;

        if (str == "-pgn") {
            pgnPath = std::string(argv[++i]);
            continue;
        }
        if (str == "-db") {
            dbPath = std::string(argv[++i]);
            continue;
        }
        if (str == "-cpu") {
            cpu = std::atoi(argv[++i]);
            continue;
        }
    }
    
    auto ok = true;
    
    if (benchMode) {
        if (dbPath.empty()) {
            ok = false;
        } else {
            if (benchMode == 1) {
                oc.bench(dbPath);
            } else {
                oc.benchMatchingMoves(dbPath);
            }
        }
    } else {
        if (pgnPath.empty() || dbPath.empty()) {
            ok = false;
        } else {
            oc.convertPgn2Sql(pgnPath, dbPath, cpu);
        }
    }

    if (!ok) {
        std::cerr << "Usage:" << std::endl;
        std::cerr << " ocgdb [<options>]" << std::endl;
        std::cerr << std::endl;
        std::cerr << " -pgn <file>           PGN game database file" << std::endl;
        std::cerr << " -db <file>            create database, extension should be .ocgdb.db3" << std::endl;
        std::cerr << "                       use :memory: to create in-memory database" << std::endl;
        std::cerr << " -cpu <n>              number of threads, omit for all cores, works with -pgn" << std::endl;
        std::cerr << " -bench                benchmarch querying games speed, works with -db" << std::endl;
        std::cerr << " -benchmoves           benchmarch querying game-moves matching speed, works with -db" << std::endl;
        std::cerr << std::endl;
        std::cerr << "Examples:" << std::endl;
        std::cerr << " ocgdb -pgn c:\\games\\big.png -db c:\\db\\big.ocgdb.db3 -cpu 4" << std::endl;
        std::cerr << " ocgdb -pgn c:\\games\\big.png -db :memory:" << std::endl;
        std::cerr << " ocgdb -bench -db c:\\db\\big.ocgdb.db3" << std::endl;
        std::cerr << " ocgdb -benchmoves -db c:\\db\\big.ocgdb.db3" << std::endl;
    }
    return 1;
}
