----------------------------------------------------------------------------------
-- Engineer: Michal Holeša
-- 
-- Create Date: 12.04.2026 18:03:15
-- Module Name: Testovanie BCD citaca
-- Project Name: IVH_podukol_2
-- Description: UUT1 = citac jednotek (0-9)
--              UUT2 = citac desiatok  (0-9)
--              Faza 1: 00 -> 99
--              Faza 2: 90 -> 00
--              CLK perioda: 10ns
--              Jeden krok: 50ns
--              Reset: 20ns
--              t(N) = reset + N * 50ns
----------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY counter_tb IS
END counter_tb;

ARCHITECTURE behavior OF counter_tb IS

    COMPONENT counter IS
        PORT (
            CLK    : IN  STD_LOGIC;
            RESET  : IN  STD_LOGIC;
            DIR    : IN  STD_LOGIC;
            EN     : IN  STD_LOGIC;
            Q      : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
            EN_OUT : OUT STD_LOGIC
        );
    END COMPONENT;

    SIGNAL clk     : STD_LOGIC := '0';

    SIGNAL reset1  : STD_LOGIC := '0';
    SIGNAL reset2  : STD_LOGIC := '0';
    SIGNAL dir     : STD_LOGIC := '0';
    SIGNAL mux_sel : STD_LOGIC := '0';

    SIGNAL en1     : STD_LOGIC := '0';
    SIGNAL en2     : STD_LOGIC;

    SIGNAL q1      : STD_LOGIC_VECTOR(3 DOWNTO 0);
    SIGNAL q2      : STD_LOGIC_VECTOR(3 DOWNTO 0);
    SIGNAL en1_out : STD_LOGIC;
    SIGNAL en2_out : STD_LOGIC;

    SIGNAL value   : INTEGER;

BEGIN

    JEDNOTKY : counter PORT MAP (
        CLK    => clk,
        RESET  => reset1,
        DIR    => dir,
        EN     => en1,
        Q      => q1,
        EN_OUT => en1_out
    );

    DESIATKY : counter PORT MAP (
        CLK    => clk,
        RESET  => reset2,
        DIR    => dir,
        EN     => en2,
        Q      => q2,
        EN_OUT => en2_out
    );

    en2 <= en1_out WHEN mux_sel = '0' ELSE en1;

    value <= to_integer(unsigned(q2)) * 10 + to_integer(unsigned(q1));

    clk <= NOT clk AFTER 5 ns;

    process
    begin
        en1 <= '1';
        WAIT FOR 10 ns;
        en1 <= '0';
        WAIT FOR 40 ns;
    END PROCESS;

    process
    begin
        reset1 <= '1';
        reset2 <= '1';
        dir    <= '0';
        mux_sel <= '0';
        
        WAIT FOR 20 ns;
        
        reset1 <= '0';
        reset2 <= '0';

        WAIT FOR 940 ns;
        ASSERT value = 19
            REPORT "CHYBA: Ocakavana hodnota 19, skutocna hodnota: "& INTEGER'IMAGE(value)SEVERITY ERROR;

        WAIT FOR 60 ns;
        ASSERT value = 20
            REPORT "CHYBA: Ocakavana hodnota 20, skutocna: "& INTEGER'IMAGE(value)SEVERITY ERROR;

        WAIT FOR 3940 ns;
        ASSERT value = 99
            REPORT "CHYBA: Ocakavana hodnota 99 po pocitani nahor, skutocna: "& INTEGER'IMAGE(value)SEVERITY ERROR;

        reset1  <= '1';
        dir     <= '1';
        mux_sel <= '1';

        WAIT FOR 450 ns;
        ASSERT value = 0
            REPORT "CHYBA: Ocakavana hodnota 0 po odpocitavani nadol po desiatkach, skutocna: "& INTEGER'IMAGE(value)SEVERITY ERROR;

        ASSERT FALSE
            REPORT "HOTOVO: Vsetky testy presli uspesne"
            SEVERITY NOTE;

        WAIT;
    END PROCESS;

END behavior;