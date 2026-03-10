#include "add_conditions.h"
#include <stddef.h>
#include <stdio.h>
#include "cnf.h"
#include "parse.h"

//
// LOGIN: xholesm00
//

/** Funkce demonstrující vytvoření nové (arbitrárně vybrané) klauzule
 * ve tvaru "A_{0,1,2} || -B_{0, 1, 2}" do výrokové formule
 * @param formula výroková formule, do níž bude klauzule přidána
 */
void conditions_example(CNF *formula)
{
    assert(formula != NULL);

    // vytvoření nové klauzule
    Clause *cl = create_new_clause(formula);

    // přidání proměnné A_{0,1,2} do klauzule
    // proměnná říká, že ulice spojující rozcestí 0 a 1 bude opravována v první fázi v den s indexem 2
    // cl - klauzule, do níž přidáváme literál
    // true - značí, že přidaný literál je pozitivní proměnná
    // FITST_PHASE_FLAG - značí, že aktuální proměnná je pro první fázi opravy
    // 0 - značí výchozí rozcestí s indexem 0
    // 1 - značí cílové rozcestí s indexem 1
    // 2 - značí den s indexem 2
    add_literal_to_clause(cl, true, FIRST_PHASE_FLAG, 0, 1, 2);

    // přidání proměnné -B_{0,1,2} do klauzule
    // proměnná říká, že ulice spojující rozcestí 0 a 1 nebude opravována v druhé fázi v den s indexem 2
    // cl - klauzule, do níž přidáváme literál
    // false - značí, že přidaný literál je negativní proměnná
    // SECOND_PHASE_FLAG - značí, že aktuální proměnná je pro druhou fázi opravy
    // 0 - značí výchozí rozcestí s indexem 0
    // 1 - značí cílové rozcestí s indexem 1
    // 2 - značí den s indexem 2
    add_literal_to_clause(cl, false, SECOND_PHASE_FLAG, 0, 1, 2);
}

/** Funkce vytvářející klauzule ošetřující podmínku 1 ze zadání
 * @param formula výroková formule, do níž bude klauzule přidána
 * @param num_of_days počet dní
 * @param num_of_crossroads počet rozcestí
 * @param num_of_streets počet ulic
 * @param neighbours seznamy sousedů
 * @param streets seznam ulic
 */
void all_streets_min_one_day_of_first_phase_roadwork(CNF *formula, unsigned num_of_days, unsigned num_of_crossroads, unsigned num_of_streets, const NeighbourLists *neighbours, const Street *streets)
{

    assert(formula != NULL);
    assert(num_of_crossroads >= 2);
    assert(num_of_days > 0);

    // Každá klauzule bude vyjadřovat podmínku, že daná silnice je opravována
    // v první fázi alespoň v jednom z dostupných dnů
    // Existuje-li například silnice (0, 1) mezi rozcestími 0 a 1, pak pro
    // n dní bude mít klauzule tvar:
    // A_{0, 1, 0} || A_{0, 1, 1} || A_{0, 1, 2} || ... || A_{0, 1, n-1},
    // což ve formátu DIMACS odpovídá klauzuli:
    // A_{0, 1, 0} A_{0, 1, 1} A_{0, 1, 2} ... A_{0, 1, n-1} 0
    // (přičemž každá z proměnných A_{0, 1, d} je zakódována jako jedno celé číslo)
    for (unsigned street_idx = 0; street_idx < num_of_streets; ++street_idx)
    {
        Street street = streets[street_idx];
        Clause *cl = create_new_clause(formula);

        // Pro každou ulici vyjádříme podmínku, že musí být opravena v první fázi v alespoň jednom dni

        for (unsigned day = 0; day < num_of_days; ++day)
        {
            add_literal_to_clause(cl, true, FIRST_PHASE_FLAG, street.source, street.destination, day);
        }
    }

    /* Alternativní varianta:

    // iterujeme přes veškerá výstupní rozcestí
    for(unsigned src = 0; src < num_of_crossroads; ++src) {

        // iterujeme přes veškerá vstupní rozcestí, přičemž dle zadání vždy platí dst > src
        for(unsigned dst = src + 1; dst < num_of_crossroads; ++dst) {

            // zajímají nás pouze ty dvojice rozcestí, mezi nimiž skutečně existuje silnice
            if(are_neighbours(neighbours, src, dst)) {

                // dále budeme iterovat přes dny, tudíž zde vytvoříme čerstvou klauzuli
                Clause *cl = create_new_clause(formula);

                // iterujeme přes dny, abychom mohli přidávat odpovídající proměnné
                // do nově vytvořené klauzule
                for(unsigned day = 0; day < num_of_days; ++day) {
                    // přidání proměnné x_{src, dst, day} do klauzule cl
                    // příznaky vyjadřují, že jde o proměnnou v pozitivním tvaru a že odpovídá první fázi oprav
                    add_literal_to_clause(cl, true, FIRST_PHASE_FLAG, src, dst, day);
                }
            }
        }
    }*/
}

/** Funkce vytvářející klauzule ošetřující podmínku 2 ze zadání
 * @param formula výroková formule, do níž bude klauzule přidána
 * @param num_of_days počet dní
 * @param num_of_crossroads počet rozcestí
 * @param num_of_streets počet ulic
 * @param neighbours seznamy sousedů
 * @param streets seznam ulic
 */
void all_streets_max_one_day_of_first_phase_roadwork(CNF *formula, unsigned num_of_days, unsigned num_of_crossroads, unsigned num_of_streets, const NeighbourLists *neighbours, const Street *streets)
{
    assert(formula != NULL);

    // Prejde vsetky ulice
    for (int i = 0; i < num_of_streets; i++)
    {
        Street street = streets[i];

        for (int day = 0; i < num_of_days; day++)
        {
            for (int next_day = day + 1; next_day < num_of_days; next_day++)
            {
                Clause *clause = create_new_clause(formula);

                add_literal_to_clause(clause, false, FIRST_PHASE_FLAG, street.source, street.destination, day);
                add_literal_to_clause(clause, false, FIRST_PHASE_FLAG, street.source, street.destination, next_day);
            }
        }
    }
}

/** Funkce vytvářející klauzule ošetřující podmínku 3 ze zadání
 * @param formula výroková formule, do níž bude klauzule přidána
 * @param num_of_days počet dní
 * @param num_of_crossroads počet rozcestí
 * @param num_of_streets počet ulic
 * @param neighbours seznamy sousedů
 * @param streets seznam ulic
 */
void second_phase_follows_first_immediately(CNF *formula, unsigned num_of_days, unsigned num_of_crossroads, unsigned num_of_streets, const NeighbourLists *neighbours, const Street *streets)
{
    assert(formula != NULL);

    for (int idx = 0; idx < num_of_streets; idx++)
    {
        Street street = streets[idx];

        // V posledny den nemoze byt prva faza
        Clause *clause_first_phase_last_day = create_new_clause(formula);
        add_literal_to_clause(clause_first_phase_last_day, false, FIRST_PHASE_FLAG, street.source, street.destination, num_of_days - 1);

        // V prvy den nemoze byt druha faza
        Clause *claus_second_phase_first_day = create_new_clause(formula);
        add_literal_to_clause(claus_second_phase_first_day, false, SECOND_PHASE_FLAG, street.source, street.destination, 0);

        for (int i = 0; i < num_of_days - 1; i++)
        {
            // Rozdelime si ekvivalenciu na dve klauzule
            // Negovane A || B
            Clause *clause1 = create_new_clause(formula);
            add_literal_to_clause(clause1, false, FIRST_PHASE_FLAG, street.source, street.destination, i);
            add_literal_to_clause(clause1, true, SECOND_PHASE_FLAG, street.source, street.destination, i + 1);

            // A || negovane B
            Clause *clause2 = create_new_clause(formula);
            add_literal_to_clause(clause2, true, FIRST_PHASE_FLAG, street.source, street.destination, i);
            add_literal_to_clause(clause2, false, SECOND_PHASE_FLAG, street.source, street.destination, i + 1);
        }
    }
}

/** Funkce vytvářející klauzule ošetřující podmínku 4 ze zadání
 * @param formula výroková formule, do níž bude klauzule přidána
 * @param num_of_days počet dní
 * @param num_of_crossroads počet rozcestí
 * @param num_of_streets počet ulic
 * @param neighbours seznamy sousedů
 * @param streets seznam ulic
 */
void neighbour_streets_not_being_repaired_simultaneously(CNF *formula, unsigned num_of_days, unsigned num_of_crossroads, unsigned num_of_streets, const NeighbourLists *neighbours, const Street *streets)
{
    assert(formula != NULL);

    for (int day = 0; day < num_of_days; day++)
    {
        for (int i = 0; i < num_of_streets; i++)         //
        {                                                // Cyklus na porovnanie dvoch ulic aby sme zistili ci maju rovnake razcestie/a
            for (int j = i + 1; j < num_of_streets; j++) //
            {
                Street street1 = streets[i];
                Street street2 = streets[j];

                if (street1.source == street2.source || street1.destination == street2.destination || street1.source == street2.destination || street1.destination == street2.source)
                {
                    // Faza 1 a faza 1
                    Clause *clause1 = create_new_clause(formula);
                    add_literal_to_clause(clause1, false, FIRST_PHASE_FLAG, street1.source, street1.destination, day);
                    add_literal_to_clause(clause1, false, FIRST_PHASE_FLAG, street2.source, street2.destination, day);

                    // Faza 2 a faza 2
                    Clause *clause2 = create_new_clause(formula);
                    add_literal_to_clause(clause2, false, SECOND_PHASE_FLAG, street1.source, street1.destination, day);
                    add_literal_to_clause(clause2, false, SECOND_PHASE_FLAG, street2.source, street2.destination, day);

                    // Faza 1 a faza 2
                    Clause *clause3 = create_new_clause(formula);
                    add_literal_to_clause(clause3, false, FIRST_PHASE_FLAG, street1.source, street1.destination, day);
                    add_literal_to_clause(clause3, false, SECOND_PHASE_FLAG, street2.source, street2.destination, day);

                    // Faza 2 a faza 1
                    Clause *clause4 = create_new_clause(formula);
                    add_literal_to_clause(clause4, false, SECOND_PHASE_FLAG, street1.source, street1.destination, day);
                    add_literal_to_clause(clause4, false, FIRST_PHASE_FLAG, street2.source, street2.destination, day);
                }
            }
        }
    }
}

/** Funkce vytvářející klauzule ošetřující podmínku 5 ze zadání
 * @param formula výroková formule, do níž bude klauzule přidána
 * @param num_of_days počet dní
 * @param num_of_crossroads počet rozcestí
 * @param num_of_streets počet ulic
 * @param neighbours seznamy sousedů
 * @param streets seznam ulic
 */
void each_day_at_least_one_street_being_repaired(CNF *formula, unsigned num_of_days, unsigned num_of_crossroads, unsigned num_of_streets, const NeighbourLists *neighbours, const Street *streets)
{
    assert(formula != NULL);

    for (int day = 0; day < num_of_days; day++)
    {
        Clause *clause = create_new_clause(formula);

        for (int i = 0; i < num_of_streets; i++)
        {
            Street street = streets[i];

            add_literal_to_clause(clause, true, FIRST_PHASE_FLAG, street.source, street.destination, day);
            add_literal_to_clause(clause, true, SECOND_PHASE_FLAG, street.source, street.destination, day);
        }
    }
}

/** Funkce vytvářející klauzule ošetřující podmínku 6 ze zadání
 * @param formula výroková formule, do níž bude klauzule přidána
 * @param num_of_days počet dní
 * @param num_of_crossroads počet rozcestí
 * @param num_of_streets počet ulic
 * @param neighbours seznamy sousedů
 * @param streets seznam ulic
 */
void street_between_0_and_1_repaired_in_last_two_days(CNF *formula, unsigned num_of_days, unsigned num_of_crossroads, unsigned num_of_streets, const NeighbourLists *neighbours, const Street *streets)
{
    assert(formula != NULL);
    assert(num_of_days >= 2);

    // Ak je ulica medzi 0 a 1
    if (are_neighbours(neighbours, 0, 1))
    {
        Clause *clause1 = create_new_clause(formula);
        add_literal_to_clause(clause1, true, FIRST_PHASE_FLAG, 0, 1, num_of_days - 2); // Prva faza v predposledny den

        Clause *clause2 = create_new_clause(formula);
        add_literal_to_clause(clause2, true, SECOND_PHASE_FLAG, 0, 1, num_of_days - 1); // Druha faza v posledny den
    }
}

/** Funkce vytvářející klauzule ošetřující podmínku 7 ze zadání
 * @param formula výroková formule, do níž bude klauzule přidána
 * @param num_of_days počet dní
 * @param num_of_crossroads počet rozcestí
 * @param num_of_streets počet ulic
 * @param neighbours seznamy sousedů
 * @param streets seznam ulic
 */
void no_street_to_0_repaired_during_weekend(CNF *formula, unsigned num_of_days, unsigned num_of_crossroads, unsigned num_of_streets, const NeighbourLists *neighbours, const Street *streets)
{
    assert(formula != NULL);
    assert(num_of_days >= 5);

    for (int i = 0; i < num_of_days; i++)
    {
        // Kontrola vikendu
        if (i % 7 == 5 || i % 7 == 6)
        {
            for (int j = 0; j < num_of_streets; j++)
            {
                Street street = streets[j];

                // Ak ulica zacina alebo konci v razcesti 0
                if (street.source == 0 || street.destination == 0)
                {
                    // Nesmie sa opravovat v prvej ani druhej faze cez vikendy
                    Clause *clause1 = create_new_clause(formula);                                                   //
                    add_literal_to_clause(clause1, false, FIRST_PHASE_FLAG, street.source, street.destination, i);  //
                                                                                                                    //
                    Clause *clause2 = create_new_clause(formula);                                                   //
                    add_literal_to_clause(clause2, false, SECOND_PHASE_FLAG, street.source, street.destination, i); //
                }
            }
        }
    }
}
