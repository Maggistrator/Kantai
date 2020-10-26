#ifndef BUTTON_H
#define BUTTON_H


class Button
{
    public:
        Button(void (callback*) (StateBasedGame& g, Event& e));
        virtual ~Button();

    protected:

    private:
        void (m_callback*) (StateBasedGame& g, Event& e);
};

#endif // BUTTON_H
