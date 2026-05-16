----------------------------------------------------------------------------------
-- Engineer: Michal Holesa
-- Module Name: matrix_top_tb - Behavioral
-- Project Name: IVH_projekt
-- Description: Testbench matrix_top
--
-- Casy su skratene 50x oproti realite v matrix_top
-- POR hotovy 0.26 us (256 cyklov)
-- ST_SLOW -> FAST (10 x tick_1s = 200 ms)
-- ST_FAST -> DOWN (89 x tick_100ms = 178 ms)
-- ST_DOWN -> IMG (9 x tick_500ms = 90 ms)
-- ST_IMG -> ROT (5 x tick_1s = 100 ms)
-- ST_ROT -> ANIM (16 x tick_100ms = 32 ms)
-- ST_ANIM -> SLOW (8 x tick_100ms = 16 ms)
-- 2. SLOW -> FAST (10 x tick_1s = 200 ms)
----------------------------------------------------------------------------------

LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.NUMERIC_STD.ALL;

ENTITY matrix_top_tb IS
END matrix_top_tb;

ARCHITECTURE Behavioral OF matrix_top_tb IS

    COMPONENT matrix_top IS
        PORT (
            sysclk : IN STD_LOGIC;
            row : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
            col : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
            led : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
            rpio_22_r : out STD_LOGIC
        );
    END COMPONENT;

    -- Akcelerovana CLK perioda: 1 ns = realnych 50 ns
    CONSTANT CLK_PERIOD : time := 1 ns;

    -- Timeouty pre jednotlive prechody (s 25% rezervou)
    CONSTANT T_POR_DONE : time := 1 us;
    CONSTANT T_SLOW_TO_FAST : time := 260 ms;
    CONSTANT T_FAST_TO_DOWN : time := 230 ms;
    CONSTANT T_DOWN_TO_IMG : time := 120 ms;
    CONSTANT T_IMG_ROT_TO_SLOW : time := 200 ms;
    CONSTANT T_SLOW2_TO_FAST : time := 260 ms;

    SIGNAL sysclk_s : STD_LOGIC := '0';
    SIGNAL row_s : STD_LOGIC_VECTOR(7 DOWNTO 0);
    SIGNAL col_s : STD_LOGIC_VECTOR(7 DOWNTO 0);
    SIGNAL led_s : STD_LOGIC_VECTOR(3 DOWNTO 0);
    SIGNAL rpio_22_r_s : STD_LOGIC;

    TYPE PHASE_DEC_T IS (PH_SLOW, PH_FAST, PH_DOWN, PH_IMG_ROT_ANIM, PH_UNKNOWN);
    SIGNAL phase_dec : PHASE_DEC_T := PH_UNKNOWN;

BEGIN

    UUT : matrix_top PORT MAP (
        sysclk => sysclk_s,
        row => row_s,
        col => col_s,
        led => led_s,
        rpio_22_r => rpio_22_r_s
    );

    -- CLK generator
    sysclk_s <= NOT sysclk_s AFTER CLK_PERIOD / 2;

    -- Dekodovanie led -> faza
    phase_dec <=
        PH_SLOW WHEN led_s = "0001" ELSE
        PH_FAST WHEN led_s = "0010" ELSE
        PH_DOWN WHEN led_s = "0100" ELSE
        PH_IMG_ROT_ANIM WHEN led_s = "1000" ELSE
        PH_UNKNOWN;

    -- Hlavny testovaci proces
    P_TEST : PROCESS

        PROCEDURE check_phase (
            CONSTANT expected : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
            CONSTANT msg : IN string
        ) IS
        BEGIN
            ASSERT led_s = expected
                REPORT "CHYBA: " & msg
                SEVERITY ERROR;
        END PROCEDURE;

    BEGIN
        -- Overenie POR a pociatocneho stavu
        REPORT "matrix_top_tb: ZACIATOK TESTOVANIA" SEVERITY NOTE;

        WAIT FOR T_POR_DONE;

        check_phase("0001", "Po POR ocakavany stav ST_SLOW");
        REPORT "POR dokonceny - FSM v ST_SLOW" SEVERITY NOTE;

        ASSERT col_s /= "00000000"
            REPORT "CHYBA: COL je po POR nulovy - multiplexer nefunguje"
            SEVERITY ERROR;

        REPORT "OK COL a ROW su aktivne po POR" SEVERITY NOTE;

        -- Prechod ST_SLOW -> ST_FAST
        -- 10 x 20 ms = 200 ms
        REPORT "ST_SLOW -> ST_FAST" SEVERITY NOTE;

        WAIT UNTIL led_s = "0010" FOR T_SLOW_TO_FAST;
        check_phase("0010", "Prechod ST_SLOW -> ST_FAST nenastal vcas");
        REPORT "ST_SLOW -> ST_FAST" SEVERITY NOTE;

        -- Prechod ST_FAST -> ST_DOWN
        -- 89 x 2 ms = 178 ms
        REPORT "Prechod ST_FAST -> ST_DOWN" SEVERITY NOTE;

        WAIT UNTIL led_s = "0100" FOR T_FAST_TO_DOWN;
        check_phase("0100", "Prechod ST_FAST -> ST_DOWN nenastal");
        REPORT "ST_FAST -> ST_DOWN" SEVERITY NOTE;

        -- Prechod ST_DOWN -> ST_IMG
        -- 9 x 10 ms = 90 ms
        REPORT "Prechod ST_DOWN -> ST_IMG" SEVERITY NOTE;

        WAIT UNTIL led_s = "1000" FOR T_DOWN_TO_IMG;
        check_phase("1000", "Prechod ST_DOWN -> ST_IMG nenastal");
        REPORT "ST_DOWN -> ST_IMG" SEVERITY NOTE;

        -- ST_IMG + ST_ROT + ST_ANIM -> ST_SLOW
        -- ST_IMG: 5 x 20 ms = 100 ms
        -- ST_ROT: 16 x 2 ms = 32 ms
        -- ST_ANIM: 8 x 2 ms = 16 ms
        REPORT "ST_ANIM -> ST_SLOW" SEVERITY NOTE;

        WAIT UNTIL led_s = "0001" FOR T_IMG_ROT_TO_SLOW;
        check_phase("0001", "Navrat ST_ANIM -> ST_SLOW nenastal");
        REPORT "ST_ANIM -> ST_SLOW" SEVERITY NOTE;
        REPORT "SLOW->FAST->DOWN->IMG->ROT->ANIM->SLOW" SEVERITY NOTE;

        -- Overenie druhej slucky
        REPORT "2. prechod ST_SLOW -> ST_FAST" SEVERITY NOTE;

        WAIT UNTIL led_s = "0010" FOR T_SLOW2_TO_FAST;
        check_phase("0010", "2. prechod ST_SLOW -> ST_FAST nenastal");
        REPORT "2. ST_SLOW -> ST_FAST" SEVERITY NOTE;

        REPORT "matrix_top_tb: VSETKY TESTY PRESLI USPESNE" SEVERITY NOTE;
        WAIT;

    END PROCESS P_TEST;

END Behavioral;