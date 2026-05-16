----------------------------------------------------------------------------------
-- Company: 
-- Engineer: Michal Holeša
-- 
-- Create Date: 12.03.2026 10:25:30
-- Design Name: 
-- Module Name: matrix_pack_tb
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

use work.matrix_pack.ALL;


entity matrix_pack_tb is
end matrix_pack_tb;

architecture Behavioral of matrix_pack_tb is
begin
    process
    begin
        -- 1. Testovanie NEAREST2N
        assert NEAREST2N(6) = 8   report "Chyba: NEAREST2N(6) by malo byt 8" severity error;
        assert NEAREST2N(5) = 8   report "Chyba: NEAREST2N(5) by malo byt 8" severity error;
        assert NEAREST2N(42) = 64 report "Chyba: NEAREST2N(42) by malo byt 64" severity error;
        assert NEAREST2N(64) = 64 report "Chyba: NEAREST2N(64) by malo byt 64" severity error;
        assert NEAREST2N(0) = 1   report "Chyba: NEAREST2N(0) by malo byt 1" severity error;

        -- 2. Testovanie funkcie WLOG_REDUCE
        assert WLOG_REDUCE("0000", "1111") = '0' 
            report "Chyba: WLOG_REDUCE(""0000"", ""1111"") by malo byt '0'" severity error;
        assert WLOG_REDUCE("1100", "0011") = '0' 
            report "Chyba: WLOG_REDUCE(""1100"", ""0011"") by malo byt '0'" severity error;
        assert WLOG_REDUCE("1100", "1100") = '1' 
            report "Chyba: WLOG_REDUCE(""1100"", ""1100"") by malo byt '1'" severity error; 
        assert WLOG_REDUCE("1111", "1111") = '1' 
            report "Chyba: WLOG_REDUCE(""1111"", ""1111"") by malo byt '1'" severity error;
        assert WLOG_REDUCE("1010", "1001") = '1' 
            report "Chyba: WLOG_REDUCE(""1010"", ""1001"") by malo byt '1'" severity error;
     
        report "Koniec testov" severity note;
        
        wait;
    end process;
end Behavioral;