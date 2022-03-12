// Created by Modar Nasser on 05/03/2022.

#include <sogl/Event.hpp>

#include <utility>

using namespace sogl;

Event::Event(EventTypes data) : m_data(std::move(data))
{}