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
 * @file white_box_code.h
 * 
 * @brief Deklarace metod třídy reprezentující sufixový automat.
 *
 *
 * @author Martin Dočekal
 * @author Maksim Aparovich
 */
#ifndef SUFFIX_AUTOMATON_HPP
#define SUFFIX_AUTOMATON_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <cstddef>
#include <optional>

/**
 * @brief Stav sufixového automatu
 */
struct State {
    size_t len; ///< Délka nejdelšího suffixu reprezentovaného tímto stavem
    std::optional<size_t> link; ///< Odkaz na nejdelší suffix, který je v jiné třídě ekvivalence endpos
    std::unordered_map<char, size_t> next; ///< Přechody pro každý prvek

    /**
     * @brief Konstruktor nového objektu State
     *
     */
    State() : len(0), link(std::nullopt) {}
};


/**
 * @brief Třída sufixového automatu
 */
class SuffixAutomaton {
private:
    std::vector<State> states; ///< Stavy automatu
    size_t last; ///< Index stavu reprezentujícího celý řetězec

public:

    /**
     * @brief Konstruktor nového prázdného objektu sufixového automatu
     *
     */
    SuffixAutomaton();

    /**
     * @brief Konstruktor nového objektu sufixového automatu
     *
     * @param seq[in] Počáteční sekvence pro vytvoření automatu
     */
    SuffixAutomaton(const std::string& seq) : SuffixAutomaton() {
        add_sequence(seq);
    }

    /**
     * @brief Přidání prvku do automatu
     *
     * @param elem[in] Prvek k přidání
     */
    void add_element(char elem);

    /**
     * @brief Přidání sekvence elementů do automatu
     *
     * @param seq[in] Sekvence k přidání
     */
    void add_sequence(const std::string& seq);

    /**
     * @brief Krok z daného stavu pomocí prvku
     *
     * @param state_index[in] Index stavu
     * @param elem[in] Prvek pro přechod
     * @param next_state[out] Index následujícího stavu, pokud přechod existuje, jinak nedefinováno
     * @return true Pokud přechod existuje
     * @throw std::out_of_range Pokud je state_index mimo rozsah
     */
    bool step(const size_t state_index, const char& elem, size_t& next_state) const;

    /**
     * @brief Získání elementů, ze kterých je možný přechod z daného stavu
     *
     * @param state_index[in] Index stavu
     * @return Mapa přechodů z daného stavu, kde klíčem je prvek a hodnotou index následujícího stavu
     * @throw std::out_of_range Pokud je state_index mimo rozsah
     */
    const std::unordered_map<char, size_t>& next(const size_t state_index) const;

    /**
     * @brief Kontrola, zda automat obsahuje danou sekvenci
     *
     * @param seq[in] Sekvence ke kontrole
     * @return true Pokud je sekvence obsažena v automatu
     */
    bool contains(const std::string& seq) const;

    /**
    * @brief Získání topologického uspořádání stavů
    *    Složitost: O(n), kde n je počet stavů v automatu.
    *
    * @return std::vector<size_t> Topologické uspořádání indexů stavů
    */
    std::vector<size_t> topological_sort() const;

    /**
     * @brief Získání nejdelšího přímého pokračování z daného stavu
     *  Nejdelší přímé pokračování je definováno jako sekvence přechodů ze stavů mající právě jeden odchozí přechod, počínaje daným stavem.
     *
     * @param state_index[in] Index stavu
     * @return std::string Nejdelší přímé pokračování - sekvence elementů
     */
    std::string longest_direct_continuation(const size_t state_index) const;

    /**
     * @brief Získání počtu stavů v automatu
     *
     * @return size_t Počet stavů
     */
    size_t size() const;

    /**
     * @brief Vyčištění automatu
     *
     */
    void clear();

    /**
     * @brief Získání stavu na daném indexu
     *
     * @param index[in] Index stavu
     * @return const State& Stav na daném indexu
     * @throw std::out_of_range Pokud je index mimo rozsah
     */
    const State& get_state(size_t index) const ;

};

#endif // SUFFIX_AUTOMATON_HPP

/*** Konec souboru white_box_code.h ***/
