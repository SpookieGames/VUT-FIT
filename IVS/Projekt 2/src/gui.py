#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""!
@file gui.py
@brief GUI for INTERCALCULATOR, Graphical user interface implementation using PyQt5.
@date 3.4.2026
@author Michal Holesa, Adrian Stanik 
"""

import os
import sys

from PyQt5.QtWidgets import (QApplication, QMainWindow, QWidget, QLabel, 
                             QVBoxLayout, QGridLayout, QTextEdit, QDialog, 
                             QPushButton, QMessageBox, QAction, QShortcut)
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QFont, QTextCursor, QKeySequence, QIcon, QPixmap, QFontMetrics

from infixtopost import InfixToPostFix, eval_postfix

##
#@brief Reads the application version string from a VERSION file.
# Searches the PyInstaller bundle directory, repo root, and script directory in that order.
#@return Version string from the first readable VERSION file, or "unknown" if none is found.
#
def read_version():
    candidates = []

    if getattr(sys, "frozen", False) and hasattr(sys, "_MEIPASS"):
        candidates.append(os.path.join(sys._MEIPASS, "VERSION"))

    here = os.path.dirname(os.path.abspath(__file__))
    repo_root = os.path.abspath(os.path.join(here, ".."))
    candidates.append(os.path.join(repo_root, "VERSION"))

    candidates.append(os.path.join(here, "VERSION"))

    for version_path in candidates:
        try:
            with open(version_path, "r", encoding="utf-8") as f:
                v = f.read().strip()
                if v:
                    return v
        except OSError:
            pass

    return "unknown"

##
#@brief Resolves a path to a bundled resource file.
# Works in both normal and PyInstaller frozen environments by switching the base directory.
#@param parts One or more path components joined after the base directory.
#@return Absolute path to the resource.
#
def resource_path(*parts):
    if getattr(sys, "frozen", False):
        base = sys._MEIPASS
    else:
        base = os.path.dirname(__file__)
    return os.path.join(base, *parts)

##
#@brief Main window class for the INTERCALCULATOR application.
# Inherits from QMainWindow to provide a standard application window frame.
#
class CalculatorGUI(QMainWindow):

    ##
    #@brief Initializes the main calculator window, setting its title and fixed size.
    #@return None
    #
    def __init__(self):
        super().__init__()
        self.setWindowIcon(QIcon(resource_path("assets", "icon-no-text.png")))
        version = read_version()
        self.setWindowTitle(f"INTERCALCULATOR v{version}")
        self.setFixedSize(600, 750)
        self.new_calculation = False 
        self.initUI()

    ##
    #@brief Constructs the user interface: menu bar, display, and button grid.
    #@return None
    #
    def initUI(self):
        menubar = self.menuBar()
        menubar.setNativeMenuBar(False)

        help_action = QAction('Guide', self)
        help_action.triggered.connect(self.show_help)
        menubar.addAction(help_action)

        controls_action = QAction('Controls', self)
        controls_action.triggered.connect(self.show_controls)
        menubar.addAction(controls_action)

        info_action = QAction('About', self)
        info_action.triggered.connect(self.show_info)
        menubar.addAction(info_action)

        help_action.setShortcut(QKeySequence("Ctrl+H"))
        help_action.setShortcutContext(Qt.ApplicationShortcut)

        controls_action.setShortcut(QKeySequence("Ctrl+K"))
        controls_action.setShortcutContext(Qt.ApplicationShortcut)

        info_action.setShortcut(QKeySequence("Ctrl+I"))
        info_action.setShortcutContext(Qt.ApplicationShortcut)

        self.theme_btn = QPushButton('☀', self)
        self.theme_btn.setCheckable(True)
        self.theme_btn.setChecked(True)
        self.theme_btn.setFlat(True)
        self.theme_btn.setCursor(Qt.PointingHandCursor)
        self.theme_btn.setObjectName("themeToggle")
        self.theme_btn.toggled.connect(self.toggle_theme)
        menubar.setCornerWidget(self.theme_btn, Qt.TopRightCorner)
        self.toggle_theme_action = QAction(self)
        self.toggle_theme_action.setShortcut(QKeySequence("Ctrl+M"))
        self.toggle_theme_action.setShortcutContext(Qt.ApplicationShortcut)
        self.toggle_theme_action.triggered.connect(self.theme_btn.toggle)
        self.addAction(self.toggle_theme_action)

        self.central_widget = QWidget()
        self.setCentralWidget(self.central_widget)
        
        self.layout = QVBoxLayout()
        self.central_widget.setLayout(self.layout)

        self.display = QTextEdit()
        self.display.setFixedHeight(115)
        self.display.setReadOnly(True)
        self.display.setFont(QFont("Arial", 28))
        self.display.setLineWrapMode(QTextEdit.NoWrap)
        
        self.display.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        self.display.setHorizontalScrollBarPolicy(Qt.ScrollBarAsNeeded)
        self.display.setAlignment(Qt.AlignRight)
        
        self.layout.addWidget(self.display)
        self.grid = QGridLayout()
        self.grid.setSpacing(10)
        self.layout.addLayout(self.grid)

        buttons = {
            'C': (0, 0), 'DEL': (0, 1), '(': (0, 2), ')': (0, 3), '√': (0, 4),
            '7': (1, 0), '8': (1, 1), '9': (1, 2), '÷': (1, 3), 'ln': (1, 4),
            '4': (2, 0), '5': (2, 1), '6': (2, 2), '×': (2, 3), 'n!': (2, 4),
            '1': (3, 0), '2': (3, 1), '3': (3, 2), '-': (3, 3), 'xʸ': (3, 4),
            '0': (4, 0), '.': (4, 1), '=': (4, 2), '+': (4, 3), '%': (4, 4),
        }

        for btn_text, pos in buttons.items():
            btn = QPushButton(btn_text)
            btn.setFixedSize(105, 95)
            btn.setFont(QFont("Arial", 22))
            
            if btn_text in ['C', 'DEL']:
                btn.setProperty("btnClass", "control")
            elif btn_text in ['+', '-', '×', '÷', '%', 'xʸ', '√', 'ln', 'n!', '=', '(', ')']:
                btn.setProperty("btnClass", "operator")
            else:
                btn.setProperty("btnClass", "number")

            btn.clicked.connect(lambda checked, t=btn_text: self.on_button_click(t))

            self.grid.addWidget(btn, pos[0], pos[1])

        self.light_theme = """
            QPushButton#themeToggle {
                background-color: #ffffff;
                color: #222222;
                border: 2px solid #cccccc;
                border-radius: 8px;
                padding: 6px 14px;
                font-weight: bold;
            }
            QPushButton#themeToggle:hover {
                background-color: #f0f0f0;
            }
            QPushButton#themeToggle:pressed {
                background-color: #e0e0e0;
            }
            QMainWindow, QWidget { background-color: #f2f2f2; color: #111; }
                           
            QPushButton { border-radius: 8px; border: 2px solid #ccc; color: #111; }
            QPushButton[btnClass="number"] { background-color: #ffffff; }
            QPushButton[btnClass="number"]:pressed { background-color: #e0e0e0; }
                           
            QPushButton[btnClass="operator"] { background-color: #d6eaf8; }
            QPushButton[btnClass="operator"]:pressed { background-color: #aed6f1; }
                           
            QPushButton[btnClass="control"] { background-color: #fadbd8; }
            QPushButton[btnClass="control"]:pressed { background-color: #f5b7b1; }
                           
            QTextEdit { background-color: #fff; color: #111; border: 3px solid #ccc; border-radius: 8px; padding: 10px; }
            QScrollBar:horizontal { height: 12px; background-color: #f0f0f0; }
        """

        self.dark_theme = """
            QPushButton#themeToggle {
                background-color: #3a3a3a;
                color: #f5f5f5;
                border: 2px solid #666666;
                border-radius: 8px;
                padding: 6px 14px;
                font-weight: bold;
            }
            QPushButton#themeToggle:hover {
                background-color: #4a4a4a;
            }
            QPushButton#themeToggle:pressed {
                background-color: #2f2f2f;
            }
            QMenuBar {
                background-color: #2b2b2b;
                color: #eeeeee;
            }
            QMenuBar::item {
                background: transparent;
                padding: 4px 10px;
            }
            QMenuBar::item:selected {
                background: #3a3a3a;
            }

            QMainWindow, QWidget { background-color: #2b2b2b; color: #eeeeee; }
                           
            QPushButton { border-radius: 8px; border: 2px solid #555; color: #eeeeee; }
            QPushButton[btnClass="number"] { background-color: #3c3f41; }
            QPushButton[btnClass="number"]:pressed { background-color: #555555; }
                           
            QPushButton[btnClass="operator"] { background-color: #1a5276; }
            QPushButton[btnClass="operator"]:pressed { background-color: #2980b9; }
                           
            QPushButton[btnClass="control"] { background-color: #7b241c; }
            QPushButton[btnClass="control"]:pressed { background-color: #922b21; }
                           
            QTextEdit { background-color: #1e1e1e; color: #eeeeee; border: 3px solid #555; border-radius: 8px; padding: 10px; }
            QScrollBar:horizontal { height: 12px; background-color: #333333; }
        """
        
        self.toggle_theme(self.theme_btn.isChecked())
        self.setup_shortcuts()

    ##
    #@brief Binds keyboard keys to calculator functions.
    #@return None
    #
    def setup_shortcuts(self):
        self._shortcuts = []

        for key in '0123456789.+-*/%^()':
            shortcut = QShortcut(QKeySequence(key), self)
            if key == '*':
                shortcut.activated.connect(lambda k='×': self.on_button_click(k))
            elif key == '/':
                shortcut.activated.connect(lambda k='÷': self.on_button_click(k))
            elif key == '^':
                shortcut.activated.connect(lambda k='xʸ': self.on_button_click(k))
            else:
                shortcut.activated.connect(lambda k=key: self.on_button_click(k))
            self._shortcuts.append(shortcut)

        special_keys = {
            Qt.Key_Comma: '.',
            Qt.Key_Exclam: 'n!',     
            Qt.Key_S: '√',
            Qt.Key_L: 'ln',
            Qt.Key_Enter: '=',
            Qt.Key_Return: '=',
            Qt.Key_Backspace: 'DEL',
            Qt.Key_Escape: 'C',
            Qt.Key_Delete: 'C'
        }

        for key, action in special_keys.items():
            shortcut = QShortcut(QKeySequence(key), self)
            shortcut.activated.connect(lambda a=action: self.on_button_click(a))
            self._shortcuts.append(shortcut)

    ##
    #@brief Sets the display text and repositions the cursor.
    #@return None
    #@param text The string to show in the calculator display.
    #@param align_left If True, text aligns left and cursor moves to start; otherwise right-aligned with cursor at end.
    #
    def update_display(self, text, align_left=False):
        self.display.setText(text)
        
        cursor = self.display.textCursor()
        if align_left:
            self.display.setAlignment(Qt.AlignLeft)
            cursor.movePosition(QTextCursor.Start)
        else:
            self.display.setAlignment(Qt.AlignRight)
            cursor.movePosition(QTextCursor.End)
            
        self.display.setTextCursor(cursor)

    ##
    #@brief Shows a usage guide dialog explaining how to enter expressions.
    #@return None
    #
    def show_help(self):
        text = (
            "Simple guide:\n\n"
            "• Enter the mathematical expression conventionally (infix).\n"
            "• Single-operand operations (n!, ln): Enter the number first, then the operator. (e.g., '5 n!')\n"
            "• Root operation (√): Behaves like a binary operator, enter in the format 'degree √ base'. If degree is negative the formula used is base^(1/degree).\n"
            "• If you want to enter a negative number or a longer expression into an operation (like √, ln, n!), use parentheses '()'. (e.g., '3 √ (-8)' or 'ln (5+2)').\n"
            "• 'C' clears the entire display, 'DEL' deletes the last character.\n"
            "• After pressing '=', the expression is evaluated."
            "\n"
            "• Use the top-right theme button (☀/🌙) to switch between dark and light mode.\n"
        )
        QMessageBox.information(self, "Guide", text)

    ##
    #@brief Shows an About dialog with the application version and author list.
    #@return None
    #
    def show_info(self):
        version = read_version()

        dialog = QDialog(self)
        dialog.setWindowTitle("About")
        dialog.setWindowFlags(
            dialog.windowFlags() & ~Qt.WindowContextHelpButtonHint
        )

        layout = QVBoxLayout(dialog)

        title = QLabel("INTERCALCULATOR")
        title.setAlignment(Qt.AlignCenter)
        title.setStyleSheet("font-weight: bold; font-size: 18px;")
        layout.addWidget(title)

        icon_label = QLabel()
        pix = QPixmap(resource_path("assets", "icon-512.png"))
        icon_label.setPixmap(pix.scaled(96, 96, Qt.KeepAspectRatio, Qt.SmoothTransformation))
        icon_label.setAlignment(Qt.AlignCenter)
        layout.addWidget(icon_label)

        info = QLabel(
            f"Version: {version}\n\n"
            "Authors:\n"
            "• Ha Pham <xphamha00>\n"
            "• Kristián Dúžek <xduzekk00>\n"
            "• Michal Holeša <xholesm00>\n"
            "• Adrián Staník <xstania00>\n"
        )
        info.setAlignment(Qt.AlignCenter)
        layout.addWidget(info)

        dialog.exec_()
        

    ##
    #@brief Shows a dialog listing all keyboard shortcuts.
    #@return None
    #
    def show_controls(self):
        text = (
            "Keyboard Shortcuts:\n\n"
            "• 0-9, +, -, ×, ÷, %, xʸ, (, ) : Standard input\n"
            "• . or , (Comma) : Decimal point\n"
            "• ! (Exclamation) : Factorial (n!)\n"
            "• S : Root (degree √ base)\n"
            "• L : Natural logarithm (ln)\n"
            "• Enter or Return : Evaluate (=)\n"
            "• Backspace : Delete last character (DEL)\n"
            "• Escape or Delete : Clear entire display (C)\n"
            "\n"
            "• Ctrl+H : Open Guide\n"
            "• Ctrl+M : Toggle dark/light mode\n"
            "• Ctrl+K : Open Controls\n"
            "• Ctrl+I : Open About\n"
        )
        QMessageBox.information(self, "Controls", text)

    ##
    #@brief Switches the application stylesheet between dark and light mode.
    #@return None
    #@param checked True to apply the dark theme, False for light.
    #
    def toggle_theme(self, checked):
        if checked:
            self.setStyleSheet(self.dark_theme)
            self.theme_btn.setText('☀')
        else:
            self.setStyleSheet(self.light_theme)
            self.theme_btn.setText('🌙')

    ##
    #@brief Handles a button click, updating the display or evaluating the expression.
    #@return None
    #@param text Label of the clicked button (e.g. '7', '+', '=', 'DEL').
    #
    def on_button_click(self, text):
        curr = self.display.toPlainText()

        if getattr(self, 'new_calculation', False):
            self.new_calculation = False
            
            if text not in ['+', '-', '×', '÷', '%', 'xʸ', '^', '=', 'C', 'DEL']:
                self.display.clear()
                curr = ""

        if text == 'C':
            self.display.clear()
        
        elif text == 'DEL':
            stripped = curr.rstrip()
            if stripped.endswith(('!', '^', '×', '÷', '%', '+', '-', '(', ')')):
                self.update_display(stripped[:-2])
            elif stripped.endswith(('ln', '√')):
                self.update_display(stripped[:-3])
            else:
                self.update_display(stripped[:-1])
        
        elif text == '=':
            if not curr.strip():
                return
            
            try:
                eval_string = (
                    curr.replace('√', 'sqt')
                        .replace('!', 'fac')
                        .replace('^', '^')
                        .replace('×', '*')
                        .replace('÷', '/')
                )

                eval_string = eval_string.strip()
                if eval_string.startswith('- '):
                    eval_string = "0 " + eval_string
                eval_string = eval_string.replace('( - ', '( 0 - ')

                postfix = InfixToPostFix(eval_string)
                res = eval_postfix(postfix)
                
                if res == int(res):
                    res = int(res)
                else:
                    res = round(res, 10)
                    
                res_str = str(res)
                
                font_metrics = QFontMetrics(self.display.font())
                text_width = font_metrics.boundingRect(res_str).width()
                available_width = self.display.viewport().width() - 25   
                is_too_long = text_width > available_width
                    
                self.update_display(res_str, align_left=is_too_long)

                self.new_calculation = True 
            
            except Exception as e:
                QMessageBox.critical(self, "Error", f"Invalid expression:\n{e}")
        
        else:
            if text == 'xʸ':
                text = '^'
            elif text == 'n!':
                text = '!'

            ops = ['+', '-', '×', '÷', '%', '^', '√', 'ln', '!', '(', ')']
            binary_ops = ['+', '×', '÷', '%', '^']

            if not curr.strip() and text in ['+', '-', '×', '÷', '%', '^', '!']:
                self.update_display("0")
                curr = "0"

            if text == '-' and (
                not curr.strip() or curr.rstrip().endswith(('('))
            ):
                self.update_display(curr + '-')
                return
            
            if text == '+' and (
                not curr.strip() or curr.rstrip().endswith(('('))
            ):
                self.update_display(curr + '+')
                return
            
            if text == '√' and (
                not curr.strip() or curr.rstrip().endswith(('(', '+', '-', '×', '÷', '%', '^'))
            ):
                if curr and not curr.endswith(' '):
                    self.update_display(curr + " 2 √ ")
                else:
                    self.update_display(curr + "2 √ ")
                return
            
            if text == '(':
                stripped = curr.rstrip()
                if stripped and stripped[-1].isdigit():
                    self.update_display(stripped + " × ( ")
                    return
                if stripped and stripped[-1].endswith(')'):
                    self.update_display(stripped + " × ( ")
                    return

            if curr.strip() and (text in binary_ops or text in ['-', '√', 'ln', '!']):
                stripped = curr.rstrip()

                if stripped.endswith(('+', '-', '×', '÷', '%', '^', '√', 'ln', '!')):
                    if stripped.endswith('ln'):
                        stripped = stripped[:-2].rstrip()
                    else:
                        stripped = stripped[:-1].rstrip()

                    if stripped.endswith(('+', '-', '×', '÷', '%', '^', '√', 'ln', '!')):
                        if stripped.endswith('ln'):
                            stripped = stripped[:-2].rstrip()
                        else:
                            stripped = stripped[:-1].rstrip()
                            
                    self.update_display(stripped)
                    curr = self.display.toPlainText()

            if text in ops:
                if curr and not curr.endswith(' '):
                    self.update_display(curr + f" {text} ")
                else:
                    self.update_display(curr + f"{text} ")
            else:
                self.update_display(curr + text)


if __name__ == "__main__":
    if sys.platform.startswith("linux"):
        os.environ["QT_QPA_PLATFORM"] = "wayland;xcb"

    app = QApplication(sys.argv)
    window = CalculatorGUI()
    window.show()
    sys.exit(app.exec_())
    app = QApplication(sys.argv)
    window = CalculatorGUI()
    window.show()
    sys.exit(app.exec_())