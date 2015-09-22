# CMakeLists pour la compilation de COCO/R
project(COCO_R)

set(COCO_R_SRC
    CocoSourcesCPP/Action.cpp
    CocoSourcesCPP/ArrayList.cpp
    CocoSourcesCPP/BitArray.cpp
    CocoSourcesCPP/CharClass.cpp
    CocoSourcesCPP/CharSet.cpp
    CocoSourcesCPP/Coco.cpp
    CocoSourcesCPP/Comment.cpp
    CocoSourcesCPP/DFA.cpp
    CocoSourcesCPP/Generator.cpp
    CocoSourcesCPP/HashTable.cpp
    CocoSourcesCPP/Melted.cpp
    CocoSourcesCPP/Node.cpp
    CocoSourcesCPP/Parser.cpp
    CocoSourcesCPP/ParserGen.cpp
    CocoSourcesCPP/Position.cpp
    CocoSourcesCPP/Scanner.cpp
    CocoSourcesCPP/SortedList.cpp
    CocoSourcesCPP/State.cpp
    CocoSourcesCPP/StringBuilder.cpp
    CocoSourcesCPP/Symbol.cpp
    CocoSourcesCPP/Tab.cpp
    CocoSourcesCPP/Target.cpp
)

set(COCO_R_HEADERS
    CocoSourcesCPP/Action.h
    CocoSourcesCPP/ArrayList.h
    CocoSourcesCPP/BitArray.h
    CocoSourcesCPP/CharClass.h
    CocoSourcesCPP/CharSet.h
    CocoSourcesCPP/Comment.h
    CocoSourcesCPP/DFA.h
    CocoSourcesCPP/Generator.h
    CocoSourcesCPP/Graph.h
    CocoSourcesCPP/HashTable.h
    CocoSourcesCPP/Melted.h
    CocoSourcesCPP/Node.h
    CocoSourcesCPP/Parser.h
    CocoSourcesCPP/ParserGen.h
    CocoSourcesCPP/Position.h
    CocoSourcesCPP/Scanner.h
    CocoSourcesCPP/Sets.h
    CocoSourcesCPP/SortedList.h
    CocoSourcesCPP/State.h
    CocoSourcesCPP/StringBuilder.h
    CocoSourcesCPP/Symbol.h
    CocoSourcesCPP/Tab.h
    CocoSourcesCPP/Target.h
)

if(MINGW)
set(EXTRA_SRC wmain.cpp)
endif(MINGW)

add_executable(cococpp ${COCO_R_HEADERS} ${COCO_R_SRC} ${EXTRA_SRC})
