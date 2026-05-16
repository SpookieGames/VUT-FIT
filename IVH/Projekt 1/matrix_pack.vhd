----------------------------------------------------------------------------------
-- Company: 
-- Engineer: Michal Holeša
-- 
-- Create Date: 9.03.2026 21:41:52
-- Design Name: 
-- Module Name: matrix_pack - package
-- Project Name: IVH_podukol_1
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

package matrix_pack is

    -- Deklaracie
    type STATE_T is (S_BCD_NORM, S_BCD_INV, S_ALT);
    function WLOG_REDUCE (DATA_A : std_logic_vector; DATA_B : std_logic_vector) return std_logic;
    function NEAREST2N (DATA : in natural) return natural;
    --
    
end package matrix_pack;

package body matrix_pack is
    -- Implementacia WLOG_REDUCE
    function WLOG_REDUCE (DATA_A : std_logic_vector; DATA_B : std_logic_vector) return std_logic is
        variable result : std_logic := '0';
    begin
        -- Rozna velkost
        assert (DATA_A'length = DATA_B'length) 
            report "WLOG_REDUCE: Vektory DATA_A a DATA_B musia mat rovnaku dlzku!" 
            severity failure;

        -- Prazdne vektory
        assert (DATA_A'length /= 0)
            report "WLOG_REDUCE: Vektor DATA_A nesmie byt prazdny" 
            severity failure;
        assert (DATA_B'length /= 0)
            report "WLOG_REDUCE: Vektor DATA_B nesmie byt prazdny" 
            severity failure;
        
        -- Logicky AND a nasledna OR redukcia
        for i in 0 to DATA_A'length - 1 loop
            result := result or (DATA_A(DATA_A'low + i) and DATA_B(DATA_B'low + i));
        end loop;
        
        return result;
    end WLOG_REDUCE;
    --
    
    -- Implementacia NEAREST2N
    function NEAREST2N (DATA : in natural) return natural is
        variable res : natural := 1;
    begin
        -- Nasobime 2ma, pokial nie je vysledok vacsi alebo rovny vstupu
        while res < DATA loop
            res := res * 2;
        end loop;
        return res;
    end NEAREST2N;
    --
    
end package body matrix_pack;
