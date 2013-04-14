// Queue class declaration
template<class itemType>
class queueType
{
 // member functions
 public:
  queueType(int);
  ~queueType();
  void makeEmpty();
  bool isEmpty()const;
  bool isFull()const;
  bool enqueue(itemType item);
  bool dequeue(itemType &item);

 // data members
 private:
  int front;
  int rear;
  itemType *items;
  int maxQue;
};

template<class itemType>
queueType<itemType>::queueType(int max)
{
 maxQue = max+1;
 front = maxQue-1;
 rear = maxQue-1;
 items = new itemType[maxQue];
}

template<class itemType>
queueType<itemType>::~queueType()
{
 delete [] items;
}

template<class itemType>
void queueType<itemType>::makeEmpty()
{
 front = maxQue-1;
 rear = maxQue-1;
}

template<class itemType>
bool queueType<itemType>::isEmpty()const
{
 return (rear == front);
}

template<class itemType>
bool queueType<itemType>::isFull()const
{
 return ((rear+1) % maxQue == front);
}

template<class itemType>
bool queueType<itemType>::enqueue(itemType newItem)
{
 // if not full insert item
 if(!isFull())
 {
  rear = (rear+1) % maxQue;
  items[rear] = newItem;
  return 0;
 }
 // otherwise buffer is full, return false
 else
  return -1;
}

template<class itemType>
bool queueType<itemType>::dequeue(itemType &item)
{
 // if buffer is not empty, return true
 if(!isEmpty())
 {
  front = (front+1) % maxQue;
  item = items[front];
  return 0;
 }
 // otherwise buffer is empty, return false
 else
  return -1;
}
