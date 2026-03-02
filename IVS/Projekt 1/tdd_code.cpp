//======= Copyright (c) 2025, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - graph
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     Michal Holeša <xholesm00@stud.fit.vutbr.cz>
// $Date:       $2025-02-19
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author Martin Dočekal
 * @author Karel Ondřej
 * @author Michal Holeša
 *
 * @brief Implementace metod tridy reprezentujici graf.
 */

#include "tdd_code.h"

Graph::Graph() {}

Graph::~Graph() { clear(); }

std::vector<Node *> Graph::nodes()
{
    std::vector<Node *> nodes;

    // Kopirovanie z triednej premennej danej v tdd_code.h (a_nodes) do lokalnej premennej nodes
    for (int i = 0; i < a_nodes.size(); i++)
    {
        nodes.push_back(a_nodes[i]);
    }

    return nodes;
}

std::vector<Edge> Graph::edges() const
{
    std::vector<Edge> edges;

    // Kopirovanie z triednej premennej danej v tdd_code.h (a_edges) do lokalnej premennej edges
    for (int i = 0; i < a_edges.size(); i++)
    {
        edges.push_back(a_edges[i]);
    }

    return edges;
}

Node *Graph::addNode(size_t nodeId)
{
    // Overenie ci sa uz uzol nenachadza v grafe
    for (int i = 0; i < a_nodes.size(); i++)
    {
        if (a_nodes[i]->id == nodeId)
        {
            return nullptr;
        }
    }

    // Vytvorenie noveho uzla
    Node *newNode = new Node;
    newNode->id = nodeId;
    newNode->color = 0; // Predvolena farba 0 (nevyfarbene)

    a_nodes.push_back(newNode); // Pridanie na koniec zoznamu
    return newNode;
}

bool Graph::addEdge(const Edge &edge)
{
    // Pri rovnakom uzle na koncoch preskocime
    if (edge.a == edge.b)
    {
        return false;
    }

    // Ak graf uz obsahuje hranu, preskocime ju
    if (containsEdge(edge))
    {
        return false;
    }

    // Pri uzloch co este neexistuju ich vytvorime
    if (getNode(edge.a) == nullptr)
    {
        addNode(edge.a);
    }
    if (getNode(edge.b) == nullptr)
    {
        addNode(edge.b);
    }

    a_edges.push_back(edge); // Pridanie na koniec zoznamu
    return true;
}

// Funkcia s for cyklom na pridanie viacerych hran naraz
void Graph::addMultipleEdges(const std::vector<Edge> &edges)
{
    for (int i = 0; i < edges.size(); i++)
    {
        addEdge(edges[i]);
    }
}

// For cyklus na najdenie ID zhody
Node *Graph::getNode(size_t nodeId)
{
    for (int i = 0; i < a_nodes.size(); i++)
    {
        if (a_nodes[i]->id == nodeId)
        {
            return a_nodes[i];
        }
    }

    return nullptr;
}

// For cyklus na zistenie ci graf obsahuje danu hranu
bool Graph::containsEdge(const Edge &edge) const
{
    for (int i = 0; i < a_edges.size(); i++)
    {
        if (a_edges[i] == edge)
        {
            return true;
        }
    }

    return false;
}

void Graph::removeNode(size_t nodeId)
{
    // Overenie ci uzol vobec existuje
    if (getNode(nodeId) == nullptr)
    {
        throw std::out_of_range("Uzol neexistuje");
    }

    // Zmazanie hran ktore obsahuju mazany uzol, odzadu aby sa neposuval index
    for (int i = a_edges.size() - 1; i >= 0; i--)
    {
        if (a_edges[i].a == nodeId || a_edges[i].b == nodeId)
        {
            a_edges.erase(a_edges.begin() + i);
        }
    }

    // Zmazanie uzla z pamate a vektora
    for (int i = 0; i < a_nodes.size(); i++)
    {
        if (a_nodes[i]->id == nodeId)
        {
            delete a_nodes[i];                  // Zmazanie z pamate
            a_nodes.erase(a_nodes.begin() + i); // Ostranenie uzla z vektora
            break;
        }
    }
}

void Graph::removeEdge(const Edge &edge)
{
    // Prejdenie vsetkych hran
    for (int i = 0; i < a_edges.size(); i++)
    {
        if (a_edges[i] == edge)
        {
            a_edges.erase(a_edges.begin() + i);
            return;
        }
    }

    throw std::out_of_range("Hrana neexistuje");
}

size_t Graph::nodeCount() const
{
    return a_nodes.size();
}

size_t Graph::edgeCount() const
{
    return a_edges.size();
}

size_t Graph::nodeDegree(size_t nodeId) const
{
    // Obchadzanie getNode nakolko sa to compileru nepacilo kvoli const
    bool exists = false;
    for (size_t i = 0; i < a_nodes.size(); i++)
    {
        if (a_nodes[i]->id == nodeId)
        {
            exists = true;
            break;
        }
    }

    if (!exists)
    {
        throw std::out_of_range("Uzol neexistuje"); // Vypisanie chyby ak sa uzol nenasiel v grafe
    }

    int degree = 0;
    for (int i = 0; i < a_edges.size(); i++)
    {
        if (a_edges[i].a == nodeId || a_edges[i].b == nodeId)
        {
            degree++;
        }
    }
    return degree;
}

size_t Graph::graphDegree() const
{
    int maximum = 0;

    for (int i = 0; i < a_nodes.size(); i++)
    {
        int degree = nodeDegree(a_nodes[i]->id);
        if (degree > maximum)
        {
            maximum = degree;
        }
    }
    return maximum;
}

void Graph::coloring()
{
    // Prejdenie cez vsetky uzly
    for (int i = 0; i < a_nodes.size(); i++)
    {
        int color = 1; // Zacneme farbou 1 nakolko 0 je predvolena farba pre vseky uzly
        bool check = false;

        // Cyklus while bezi dokym nenajdeme vhodnu farbu
        while (!check)
        {
            check = true;
            for (int j = 0; j < a_edges.size(); j++)
            {
                Node *neighbor;

                // Zistenie ci hrana vedie z alebo do uzla
                if (a_edges[j].a == a_nodes[i]->id)
                {
                    neighbor = getNode(a_edges[j].b);
                }
                else if (a_edges[j].b == a_nodes[i]->id)
                {
                    neighbor = getNode(a_edges[j].a);
                }

                if (neighbor->color == color)
                {
                    check = false;
                    color++;
                    break;
                }
            }
        }
        a_nodes[i]->color = color;
    }
}

void Graph::clear()
{
    // Uvolnime vsetky uzly
    for (int i = 0; i < a_nodes.size(); i++)
    {
        delete a_nodes[i];
    }

    // Vyprazdnenie vektorov
    a_nodes.clear();
    a_edges.clear();
}

/*** Konec souboru tdd_code.cpp ***/
