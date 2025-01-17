#include <cnoid/Plugin>
#include <cnoid/ItemList>
#include <cnoid/RootItem>
#include <cnoid/BodyItem>
#include <cnoid/Item>
#include <cnoid/Body>
#include <cnoid/ItemTreeView>
#include <cnoid/MenuManager>
#include <cnoid/MessageView>
#include <cnoid/SceneDrawables>
#include <cnoid/PointSetItem>
#include <cnoid/EigenUtil>

#include <iostream>
#include <queue>

using namespace cnoid;

class GeneratePointCloudPlugin : public Plugin
{
public:
    GeneratePointCloudPlugin() : Plugin("GeneratePointCloud")
    {
        require("Body");
    }

    virtual bool initialize() override
    {
        Action *menuItem = menuManager().setPath("/View").addItem("Hello World");
        menuItem->sigTriggered().connect([&]()
                                         { onHelloWorldTriggered(); });
        return true;
    }

private:
    static bool checkedItemFilter(cnoid::Item *item)
    {
        return item->isChecked();
    }
    void onHelloWorldTriggered()
    {
        auto world = cnoid::RootItem::instance()->childItem();

        std::vector<cnoid::Body *> bodies;
        std::queue<cnoid::Item *> q;
        q.push(world);

        while (not q.empty())
        {
            auto parent = q.front();
            q.pop();

            auto children = parent->childItems(checkedItemFilter);
            for (auto c : children)
            {
                q.push(c);
            }

            cnoid::ItemList<cnoid::BodyItem> new_bodies(children);
            for (auto b : new_bodies)
            {
                MessageView::instance()->putln(b->name() + (b->body() == nullptr ? " has not" : " has") + " body");
                if (b->body() != nullptr)
                {
                    bodies.push_back(b->body());
                }
            }
        }

        std::cout << "a" << std::endl;
        auto pointSetItem = new PointSetItem();
        std::cout << "a" << std::endl;
        auto pointSet = pointSetItem->pointSet();
        std::cout << "a" << std::endl;
        for (auto b : bodies)
        {
            std::cout << "b" << std::endl;
            for (auto link : b->links())
            {
                std::cout << "c" << std::endl;
                auto shapes = link->collisionShape();
                std::cout << "d" << std::endl;
                if (not shapes)
                {
                    auto msg = b->name() + ": faild loading shapes of body";
                    std::cout << msg << std::endl;
                    MessageView::instance()->putln(msg);
                    continue;
                }
                std::cout << "d" << std::endl;

                std::queue<cnoid::SgGroupPtr> queue;
                queue.push(shapes);
                std::vector<cnoid::SgNodePtr> nodes;

                while (not queue.empty())
                {
                    auto q = queue.front();
                    queue.pop();
                    for (auto j = 0; j < q->numChildren(); j++)
                    {
                        if (q->child(j)->isGroupNode())
                        {
                            queue.push(q->child(j)->toGroupNode());
                        }
                        else
                        {
                            nodes.push_back(q->child(j));
                        }
                    }
                }

                for (auto &node : nodes)
                {
                    auto shape = dynamic_pointer_cast<cnoid::SgShape>(node);
                    if (shape == nullptr)
                    {
                        auto msg = b->name() + ": faild converting shape of body";
                        std::cout << msg << std::endl;
                        MessageView::instance()->putln(msg);
                        continue;
                    }

                    std::cout << "f" << std::endl;

                    auto mesh = shape->mesh();

                    if (mesh == nullptr)
                    {
                        auto msg = b->name() + ": faild loading mesh of body";
                        std::cout << msg << std::endl;
                        MessageView::instance()->putln(msg);
                        continue;
                    }
                    std::cout << "f" << std::endl;

                    auto verticles = mesh->vertices();
                    if (verticles == nullptr)
                    {
                        auto msg = b->name() + ": faild loading verticles of body";
                        std::cout << msg << std::endl;
                        MessageView::instance()->putln(msg);
                        continue;
                    }
                    std::cout << "f" << std::endl;

                    for (auto i = 0; i < verticles->size(); i++)
                    {
                        Vector3 vertex = link->T() * verticles->at(i).cast<double>();
                        pointSet->getOrCreateVertices()->push_back(vertex.cast<float>());
                    }
                    std::cout << "f" << std::endl;

                    auto msg = b->name() + ": success";
                    std::cout << msg << std::endl;
                    MessageView::instance()->putln(msg);
                }
            }
        }

        pointSetItem->setName("pointcloud100");
        world->addSubItem(pointSetItem);
        ItemTreeView::instance()->update();
    }
};

CNOID_IMPLEMENT_PLUGIN_ENTRY(GeneratePointCloudPlugin)
