----------------------------------------------------------------------------------
-- Engineer: Michal Holesa
-- Module Name: cell - Dataflow
-- Project Name: IVH_projekt
--
-- BCD_BIT -- pixelovy bit z display_data (BCD zobrazenie cislice)
-- IMG_BIT -- pixelovy bit z img_data (vlastny obrazok, napr. smiley)
-- RIGHT_NEIGH -- IMG_BIT suseda v nasledujucom stlpci (pre animaciu posunu)
-- MODE -- STATE_T z matrix_pack: S_BCD_NORM / S_BCD_INV / S_ALT
-- INV_EN -- '1' = invertuj BCD pixel (pre S_BCD_INV fazu)
-- ANIM_EN -- '1' = zobraz praveho suseda namiesto vlastneho IMG_BIT
--
-- CELL_OUT -- '1' = LED tejto bunky svieti
----------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use work.matrix_pack.ALL;

entity cell is
    port (
        BCD_BIT : in STD_LOGIC;
        IMG_BIT : in STD_LOGIC;
        RIGHT : in STD_LOGIC;
        MODE : in STATE_T;
        INV_EN : in STD_LOGIC;
        ANIM_EN : in STD_LOGIC;
        CELL_OUT : out STD_LOGIC
    );
end cell;

architecture Dataflow of cell is
    signal bcd_display : STD_LOGIC;
    signal img_display : STD_LOGIC;

begin

    bcd_display <= BCD_BIT xor INV_EN;

    img_display <= RIGHT when ANIM_EN = '1' else IMG_BIT;

    with MODE select CELL_OUT <=
        bcd_display when S_BCD_NORM,
        bcd_display when S_BCD_INV,
        img_display when S_ALT,
        '0'         when others;

end Dataflow;