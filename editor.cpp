#include "editor.h"
#include "item.h"
#include "link.h"
#include "logger.h"

Editor* Editor::m_editor = nullptr;
std::mutex Editor::m_mutex;

Editor::Mode Editor::mode() const
{
    return m_mode;
}

void Editor::setMode(Editor::Mode newMode)
{
    LOGI("Editor::Mode={}", int(newMode));
    m_mode = newMode;
}

Editor::Editor()
    : m_mode(Mode::Idle)
{
}

std::shared_ptr<Item> Editor::selectedItem() const
{
    return m_selectedItem;
}

void Editor::setSelectedItem(const std::shared_ptr<Item>& newSelectedItem)
{
    m_selectedItem = newSelectedItem;
}

std::vector<std::shared_ptr<Link>>& Editor::links()
{
    return m_links;
}

std::vector<std::shared_ptr<Link>> Editor::links() const
{
    return m_links;
}

std::vector<std::shared_ptr<Item>>& Editor::items()
{
    return m_items;
}

std::vector<std::shared_ptr<Item>> Editor::items() const
{
    return m_items;
}

Editor* Editor::instance()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_editor == nullptr) {
        m_editor = new Editor();
    }
    return m_editor;
}
