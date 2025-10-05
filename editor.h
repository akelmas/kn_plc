#ifndef EDITOR_H
#define EDITOR_H

#include <vector>
class Item;
class Link;

class Editor {
public:
    enum class Mode : uint8_t {
        Idle,
        StartAddLink
    };

public:
    Editor(Editor& other) = delete;
    void operator=(const Editor&) = delete;
    static Editor* instance();
    Editor::Mode mode() const;
    void setMode(Editor::Mode newMode);

    std::vector<std::shared_ptr<Item>>& items();
    std::vector<std::shared_ptr<Item>> items() const;

    std::vector<std::shared_ptr<Link>>& links();
    std::vector<std::shared_ptr<Link>> links() const;

    std::shared_ptr<Item> selectedItem() const;
    void setSelectedItem(const std::shared_ptr<Item>& newSelectedItem);

private:
    Editor();
    static std::mutex m_mutex;
    static Editor* m_editor;
    Editor::Mode m_mode;
    std::shared_ptr<Item> m_selectedItem;
    std::vector<std::shared_ptr<Item>> m_items;
    std::vector<std::shared_ptr<Link>> m_links;
};

#endif // EDITOR_H
