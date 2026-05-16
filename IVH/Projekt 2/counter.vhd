----------------------------------------------------------------------------------
-- Engineer: Michal Holeša
-- 
-- Create Date: 12.04.2026 18:03:15
-- Module Name: counter - Dataflow
-- Project Name: IVH_podukol_2
----------------------------------------------------------------------------------
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.NUMERIC_STD.ALL;

ENTITY counter IS
    PORT (
        CLK    : IN  STD_LOGIC;
        RESET  : IN  STD_LOGIC;
        DIR    : IN  STD_LOGIC;
        EN     : IN  STD_LOGIC;
        Q      : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
        EN_OUT : OUT STD_LOGIC
    );
END counter;

ARCHITECTURE Dataflow OF counter IS
    SIGNAL Q_reg, q_next : UNSIGNED(3 DOWNTO 0) := (OTHERS => '0');
    SIGNAL en_out_next   : STD_LOGIC;
BEGIN

    q_next <= 
        (OTHERS => '0')   when RESET = '1' else
        (OTHERS => '0')   when EN = '1' and DIR = '0' and Q_reg = 9 else
        Q_reg + 1         when EN = '1' and DIR = '0' else
        to_unsigned(9, 4) when EN = '1' and DIR = '1' and Q_reg = 0 else
        Q_reg - 1         when EN = '1' and DIR = '1' else
        Q_reg;

    en_out_next <=
        '0' when RESET = '1' else
        '1' when EN = '1' and DIR = '0' and Q_reg = 9 else
        '1' when EN = '1' and DIR = '1' and Q_reg = 0 else
        '0';

    Q_reg  <= q_next      when rising_edge(CLK);
    EN_OUT <= en_out_next when rising_edge(CLK);

    Q <= STD_LOGIC_VECTOR(Q_reg);

END Dataflow;
