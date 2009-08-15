//==============================================================================

#ifndef ArseMouse
#define ArseMouse

#pragma once

class Mouse
{
  public:
    Mouse();
    virtual ~Mouse();

  private:
    Mouse( const Mouse & );
    Mouse & operator=( const Mouse & );

    enum MouseState
    {
        MOUSE_START,
        MOUSE_FIRST_DOWN,
        MOUSE_FIRST_UP,
        MOUSE_SECOND_DOWN,
        MOUSE_SECOND_UP,
        MOUSE_END
    };

    enum MouseAction
    {
        ACTION_NONE,
        ACTION_DRAGGING,
        ACTION_CLICKED,
        ACTION_DOUBLED
    };

  public:
    class MouseButton
    {
      public:
        MouseButton();
        virtual ~ MouseButton();

      private:
        MouseButton( const MouseButton & );
        MouseButton & operator=( const MouseButton & );

      public:
        void clear();
        void update( float dt, bool state );
        bool dragging() const;
        bool clicked() const;
        bool doubleClicked() const;
        const b2Vec2 & getDelta() const;

      private:
        MouseState m_state;
        MouseAction m_action;
        float m_timer;
        b2Vec2 m_last;
        b2Vec2 m_delta;
        b2Vec2 m_total;
    };

  public:
    void clear();
    void update( float dt );

    const MouseButton & getLeft() const;
    const MouseButton & getRight()const;
	b2Vec2 getMousePos() const;

  private:
    MouseButton m_left;
    MouseButton m_right;
};

#endif

//==============================================================================
