#pragma once

#include "Explorer/Core/Core.h"
#include "Layer.h"

#include <vector>

namespace Explorer
{
	/*
	* 普通层在列表前半部分 覆盖层在列表后半部分
	*/

	/// <summary>
	/// 层栈：存放层
	/// </summary>
	class EXPLORER_API LayerStack
	{
	private:
		std::vector<Layer*> m_Layers;			//层列表
		unsigned int m_LayerInsertIndex = 0;	//插入位置索引
	public:
		LayerStack();
		~LayerStack();

		/// <summary>
		/// 将layer添加到 插入迭代器 的位置
		/// </summary>
		/// <param name="layer">层</param>
		void PushLayer(Layer* layer);

		/// <summary>
		/// 将覆盖层添加到栈顶
		/// </summary>
		/// <param name="overlay">覆盖层</param>
		void PushOverlay(Layer* overlay);

		/// <summary>
		/// 将layer移除层栈
		/// </summary>
		/// <param name="layer">层</param>
		void PopLayer(Layer* layer);

		/// <summary>
		/// 将覆盖层移除层栈
		/// </summary>
		/// <param name="overlay">覆盖层</param>
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	};
}