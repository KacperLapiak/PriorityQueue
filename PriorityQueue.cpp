#include <iostream>
#include <math.h>

using namespace std;

class Task
{
public:
	int priority_ = 0;
	string task_ = "task pr. 0 (default)";
	Task() { }
	Task(int priority, string task) : priority_(priority), task_(task) { } 
};

class Pqueue
{
private:
	Task* heap_;
	unsigned int size_;
	unsigned int height_;

public:
	Pqueue() : size_(0), height_(0), heap_(nullptr) { }
	~Pqueue() { delete[] heap_; }
	int hSize() const { return size_; }
	int hHeight() const { return log2(size_); }
	int hNotLeafLast() const { return ((size_/2) - 1); }
	int hiLnode(const int i) const { return (i << 1) + 1; }
	int hiRnode(const int i) const { return (i << 1) + 2; }
	int hiParent(const int i) const { return (int)ceil(i/2.f) - 1; }
	Task hParent(const int i) const { return heap_[hiParent(i)]; }
	Task hvLnode(const int i) const { return heap_[hiLnode(i)]; }
	Task hvRnode(const int i) const { return heap_[hiRnode(i)]; }

	void hPushBack(const Task& newValue)
	{
		Task* buffer = new Task[size_ + 1];

		for (int i = 0; i < size_; i++)
			buffer[i] = heap_[i];

		buffer[size_] = newValue;

		size_++;
		delete[] heap_;
		heap_ = buffer;
		height_ = hHeight();
	}
	void hMaxHeapify(const int i)
	{
		int ileft = hiLnode(i);
		int iright = hiRnode(i);
		int ilargest = 0;

		if (heap_[i].priority_ < heap_[ileft].priority_ && ileft < size_) ilargest = ileft;
		else ilargest = i;
		
		if (heap_[ilargest].priority_ < heap_[iright].priority_ && iright < size_) ilargest = iright;

		if (ilargest != i)
		{
			Task buffer = heap_[i];
			heap_[i] = heap_[ilargest];
			heap_[ilargest] = buffer;

			hMaxHeapify(ilargest);
		}
	}
	void hMinHeapify(const int i)
	{
		int ileft = hiLnode(i);
		int iright = hiRnode(i);
		int ilowest = 0;

		if (heap_[i].priority_ > heap_[ileft].priority_ && ileft < size_) ilowest = ileft;
		else ilowest = i;

		if (heap_[ilowest].priority_ > heap_[iright].priority_ && iright < size_) ilowest = iright;

		if (ilowest != i)
		{
			Task buffer = heap_[i];
			heap_[i] = heap_[ilowest];
			heap_[ilowest] = buffer;

			hMinHeapify(ilowest);
		}
	}
	void hChangePrior(int i, int newPr, string mode)
	{
		if (mode == "max")
		{
			if (newPr > heap_[i].priority_)
				heap_[i].priority_ = newPr;

			while (i > 0 && heap_[hiParent(i)].priority_ < heap_[i].priority_)
			{
				Task buffer = heap_[hiParent(i)];
				heap_[hiParent(i)] = heap_[i];
				heap_[i] = buffer;
				i = hiParent(i);
			}
		}
		if (mode == "min")
		{
			if (newPr < heap_[i].priority_)
				heap_[i].priority_ = newPr;

			while (i > 0 && heap_[hiParent(i)].priority_ > heap_[i].priority_)
			{
				Task buffer = heap_[hiParent(i)];
				heap_[hiParent(i)] = heap_[i];
				heap_[i] = buffer;
				i = hiParent(i);
			}
		}
	}
	void hBuild(const string& mode)
	{
		if (mode == "max")
		{
			for (int i = hNotLeafLast(); i >= 0; i--)
				hMaxHeapify(i);
		}
		else if (mode == "min")
		{
			for (int i = hNotLeafLast(); i >= 0; i--)
				hMinHeapify(i);
		}
	}
	void hSort(const string& mode)
	{
		int buffer = size_;
		if (mode == "max")
		{
			hBuild("max");
			for (int i = buffer - 1; i > 0; i--)
			{
				Task buffer = heap_[0];
				heap_[0] = heap_[size_ - 1];
				heap_[size_ - 1] = buffer;
				size_--;
				hMaxHeapify(0);
			}
		}
		else if (mode == "min")
		{
			hBuild("min");
			for (int i = buffer - 1; i > 0; i--)
			{
				Task buffer = heap_[0];
				heap_[0] = heap_[size_ - 1];
				heap_[size_ - 1] = buffer;
				size_--;
				hMinHeapify(0);
			}
		}

		size_ = buffer;
	}
	void hInsert(const Task newValue, string mode)
	{
		hPushBack(newValue);
		if (mode == "max")
		{
			heap_[size_ - 1].priority_ = INT_MIN;
			hChangePrior(size_ - 1, newValue.priority_, "max");
		}
		else if(mode == "min")
		{
			heap_[size_ - 1].priority_ = INT_MAX;
			hChangePrior(size_ - 1, newValue.priority_, "min");
		}
	}
	Task& hTopHeapRef() { return heap_[0]; }
	Task hTopExtract(const string& mode)
	{
		Task top = heap_[0];
		heap_[0] = heap_[size_ - 1];
		size_--;
		if (mode == "max") hMaxHeapify(0);
		else if (mode == "min") hMinHeapify(0);

		return top;
	}

	void hShow() const 
	{
		for (int i = 0; i < size_; i++)
			cout << heap_[i].priority_ << " ";
	}
	void hShowGraph() const
	{
		for (int a = 0; a < height_ + 1; a++)
		{
			for (int i = pow(2, a) - 1; i < pow(2, a + 1) - 1; i++)
			{
				if (i == size_) break;
				cout << heap_[i].priority_ << " ";
			}
			cout << endl;
		}
	}

	Task& operator [](int i) { return heap_[i]; }
};

int main()
{
	Task 
	task1(15, "task pr. 15"),
	task2(25, "task pr. 25"),
	task3(10, "task pr. 10"),
	task4(55, "task pr. 55"),
	task5(85, "task pr. 85"),
	task6(75, "task pr. 75");

	Pqueue heap;
	heap.hPushBack(task1);
	heap.hPushBack(task2);
	heap.hPushBack(task3);
	heap.hPushBack(task4);
	heap.hPushBack(task5);
	heap.hPushBack(task6);

	cout << "table view: \n";
	heap.hShow();
	cout << "\n\n";

	cout << "graph view: \n";
	heap.hShowGraph();
	cout << "\n";

	cout << "max-heap: \n";
	heap.hBuild("max");
	heap.hShowGraph();
	cout << "\n";

	cout << "increase node-3(90): \n";
	heap.hChangePrior(3, 90, "max");
	heap.hShowGraph();
	cout << "\n";

	cout << "min-heap: \n";
	heap.hBuild("min");
	heap.hShowGraph();
	cout << "\n";

	cout << "min-heap node-3(5): \n";
	heap.hChangePrior(3, 5, "min");
	heap.hShowGraph();
	cout << "\n";

	cout << "top-extracted: \n";
	Task t1 = heap.hTopExtract("min");
	cout << t1.priority_ << ", " << t1.task_;
	cout << "\n\n";

	cout << "insert-element: \n";
	Task t2(0, "task pr. 0");
	cout << t2.priority_ << ", " << t2.task_;
	heap.hInsert(t2, "min");
	cout << "\n\n";

	cout << "min-heap (top-extracted): \n";
	heap.hShowGraph();
	cout << "\n";

	cout << "sort-max-heap: \n";
	heap.hSort("max");
	heap.hShow();
	cout << "\n\n";

	cout << "sort-min-heap: \n";
	heap.hSort("min");
	heap.hShow();
	cout << "\n\n";

	cout << "top: \n";
	Task t3 = heap.hTopHeapRef();
	cout << t3.priority_ << ", " << t3.task_;
	cout << "\n\n";

	cin.get();
	return 0;
}