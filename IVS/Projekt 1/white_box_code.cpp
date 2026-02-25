//======= Copyright (c) 2026, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - suffixový automat
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Authors:    Martin Dočekal <idocekal@fit.vutbr.cz>
//              Maksim Aparovich <iaparovich@fit.vut.cz>
// $Date:       $2026-02-09
//============================================================================//
/**
 * @file white_box_code.cpp
 * @author Martin Dočekal
 * @author Maksim Aparovich
 * 
 * @brief Definice metod tridy reprezentujici sufixový automat.
 */

#include "white_box_code.h"

SuffixAutomaton::SuffixAutomaton() : last(0) {
    states.emplace_back();
    states[0].len = 0;
    states[0].link = std::nullopt;
}

void SuffixAutomaton::add_element(char elem) {
    size_t cur = states.size();
    // Přidáme nový stav reprezentující řetězec rozšířený o nový prvek
    states.emplace_back();
    states[cur].len = states[last].len + 1;
    states[cur].link = 0;

    // Aktualizujeme přechody pro všechny stavy, které nemají přechod pro nový prvek
    std::optional<size_t> p = last;
    while (p != std::nullopt && !states[p.value()].next.contains(elem)) {
        // Pokud stav p nemá přechod pro elem, vytvoříme ho a posuneme se na kratší suffix aktivního stavu
        states[p.value()].next[elem] = cur;
        p = states[p.value()].link;
    }

    // Nastavení odkazu
    // Potřebujeme odkazát na nejdelší suffix, který je v jiné třídě ekvivalence endpos

    if (p == std::nullopt) {
        // Přidaný prvek nebyl doposud v řetězci.
        // Nastavíme odkaz nového stavu na počáteční stav.
        states[cur].link = 0;
    } else {
        // Existuje stav p, který má přechod pro elem. Tedy  x+elem již existuje v automatu, kde x je suffix
        // reprezentovaný stavem p.


        size_t p_val = p.value();
        size_t q = states[p_val].next[elem];
        if (states[p_val].len + 1 == states[q].len) {
            // Pokud délka stavu q je přesně o 1 větší než délka stavu p, můžeme nastavit odkaz nového stavu přímo na q
            // Důvod:
            // p je suffixem předchozího stavu (last), p má přechod pro nový elem, tento přechod vede do stavu q,
            // který reprezentuje řetězec x+elem, a zároveň délka stavu q je o 1 větší než délka stavu p
            // To znamená, že q reprezentuje řetězec, který je právě o 1 prvek delší než řetězec reprezentovaný stavem p, a tento prvek je právě elem.

            states[cur].link = q;
        } else {
            // Délka stavu q je více než o 1 větší než délka stavu p. Alespoň jeden suffix reprezentovaný stavem q je
            // příliš dlouhý, takže musíme vytvořit klon stavu q, který bude sloužit jako mezistav

            size_t clone = states.size();
            states.emplace_back();
            states[clone].len = states[p_val].len + 1;
            states[clone].next = states[q].next;
            states[clone].link = states[q].link;

            while (p != std::nullopt && states[p.value()].next[elem] == q) {
                states[p.value()].next[elem] = clone;
                p = states[p.value()].link;
            }

            states[q].link = states[cur].link = clone;
        }
    }

    last = cur;
}

void SuffixAutomaton::add_sequence(const std::string& seq) {
    for (const char elem : seq) {
        add_element(elem);
    }
}

bool SuffixAutomaton::step(const size_t state_index, const char& elem, size_t& next_state) const {
    if (state_index >= states.size()) throw std::out_of_range("State index out of range");
    const State& state = states[state_index];
    auto it = state.next.find(elem);

    if (it == state.next.end()) return false;

    next_state = it->second;
    return true;
}

const std::unordered_map<char, size_t>& SuffixAutomaton::next(const size_t state_index) const {
    if (state_index >= states.size()) throw std::out_of_range("State index out of range");
    return states[state_index].next;
}

bool SuffixAutomaton::contains(const std::string& seq) const {
    const State* current_state = &states[0];

    for (const char elem : seq) {
        auto it = current_state->next.find(elem);
        if (it == current_state->next.end()) return false;
        current_state = &states[it->second];
    }
    return true;
}

std::vector<size_t> SuffixAutomaton::topological_sort() const {
    std::vector<size_t> topo_order;

    const size_t max_len = states[last].len;  // complexity O(1)
    std::vector<std::vector<size_t>> buckets(max_len+1); // complexity: O(max_len), max_len < = 2*len(s) - 1

    for (size_t i = 0; i + 1 < states.size(); ++i) { // complexity: O(n)
        buckets[states[i].len].push_back(i);
    }


    topo_order.reserve(states.size());

    for (const auto& bucket : buckets) { // complexity: O(n)
        for (size_t state_index : bucket) {
            topo_order.emplace_back(state_index);
        }
    }

    return topo_order;
}

std::string SuffixAutomaton::longest_direct_continuation(const size_t state_index) const {
    if (state_index >= states.size()) throw std::out_of_range("State index out of range");
    std::string continuation;
    const State* current_state = &states[state_index];

    while (true) {
        if (current_state->next.size() < 1) break;
        auto it = current_state->next.begin();
        continuation.push_back(it->first);
        current_state = &states[it->second];
    }
    return continuation;
}


size_t SuffixAutomaton::size() const {
    return states.size();
}

void SuffixAutomaton::clear() {
    states.clear();
    states.emplace_back();
    states[0].len = 0;
    states[0].link = std::nullopt;
    last = 0;
}

const State& SuffixAutomaton::get_state(size_t index) const {
    if (index >= states.size()) throw std::out_of_range("Index out of range");
    return states[index];
}


/*** Konec souboru white_box_code.cpp ***/
