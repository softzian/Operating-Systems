// set up buffer characteristics
typedef int buffer_item;
#define BUFFER_SIZE 5;

template<class itemType>
class queueType
{
 // member functions
 public:
  queueType();
  ~queueType();
  void makeEmpty();
  bool isEmpty()const;
  bool isFull()const;
  bool insert_item(itemType item);
  bool remove_item(itemType &item);

 // data members
 private:
  int front;
  int rear;
  buffer_item *buffer;
  int maxQue;
};

template<class itemType>
queueType<itemType>::queueType()
{
 maxQue = BUFFER_SIZE+1;
 front = maxQue-1;
 rear = maxQue-1;
 buffer = new itemType[maxQue];
}

template<class itemType>
queueType<itemType>::~queueType()
{
 delete [] buffer;
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
bool queueType<itemType>::insert_item(itemType newItem)
{
 // if not full insert item
 if(!isFull())
 {
  rear = (rear+1) % maxQue;
  buffer[rear] = newItem;
  return 0;
 }
 // otherwise buffer is full, return false
 else
  return -1;
}

template<class itemType>
bool queueType<itemType>::remove_item(itemType &item)
{
 // if buffer is not empty, return true
 if(!isEmpty())
 {
  front = (front+1) % maxQue;
  item = buffer[front];
  return 0;
 }
 // otherwise buffer is empty, return false
 else
  return -1;
}
