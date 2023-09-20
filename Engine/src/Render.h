#ifndef _Render
#define _Render

namespace Azul
{
	class Render
	{
	protected:
		Render() = default;
		Render(const Render&) = delete;
		Render& operator = (const Render&) = delete;
		virtual ~Render() = default;

		virtual void ActivateGraphics() = 0;
		virtual void Draw() = 0;
	};
}

#endif
