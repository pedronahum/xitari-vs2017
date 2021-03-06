/* *****************************************************************************
 * Xitari
 *
 * Copyright 2014 Google Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 * *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and 
 *   the Reinforcement Learning and Artificial Intelligence Laboratory
 * Released under the GNU General Public License; see License.txt for details. 
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 */
#include "IceHockey.hpp"
#include <algorithm>

#include "../RomUtils.hpp"


using namespace ale;


IceHockeySettings::IceHockeySettings() {

    reset();
}


/* create a new instance of the rom */
RomSettings* IceHockeySettings::clone() const { 
    
    RomSettings* rval = new IceHockeySettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void IceHockeySettings::step(const System& system) {

    // update the reward
    int my_score = std::max(getDecimalScore(0x8A, &system), 0);
    int oppt_score = std::max(getDecimalScore(0x8B, &system), 0);
    int score = my_score - oppt_score;
    int reward = std::min(score - m_score, 1);
    m_reward = reward;
    m_score = score;

    // update terminal status
    int minutes = readRam(&system, 0x87);
    int seconds = readRam(&system, 0x86);
    // end of game when out of time
    m_terminal = minutes == 0 && seconds == 0;  
}


/* is end of game */
bool IceHockeySettings::isTerminal() const {

    return m_terminal;
};


/* get the most recently observed reward */
reward_t IceHockeySettings::getReward() const { 

    return m_reward; 
}


/* is an action part of the minimal set? */
bool IceHockeySettings::isMinimal(const Action &a) const {

    switch (a) {
        case PLAYER_A_NOOP:
        case PLAYER_A_FIRE:
        case PLAYER_A_UP:
        case PLAYER_A_RIGHT:
        case PLAYER_A_LEFT:
        case PLAYER_A_DOWN:
        case PLAYER_A_UPRIGHT:
        case PLAYER_A_UPLEFT:
        case PLAYER_A_DOWNRIGHT:
        case PLAYER_A_DOWNLEFT:
        case PLAYER_A_UPFIRE:
        case PLAYER_A_RIGHTFIRE:
        case PLAYER_A_LEFTFIRE:
        case PLAYER_A_DOWNFIRE:
        case PLAYER_A_UPRIGHTFIRE:
        case PLAYER_A_UPLEFTFIRE:
        case PLAYER_A_DOWNRIGHTFIRE:
        case PLAYER_A_DOWNLEFTFIRE:
            return true;
        default:
            return false;
    }   
}


/* reset the state of the game */
void IceHockeySettings::reset() {
    
    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
}

        
/* saves the state of the rom settings */
void IceHockeySettings::saveState(Serializer & ser) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void IceHockeySettings::loadState(Deserializer & ser) {
  m_reward = ser.getInt();
  m_score = ser.getInt();
  m_terminal = ser.getBool();
}

