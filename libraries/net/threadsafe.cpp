//
// Created by igor on 20.07.18.
//

#include <boost/asio/detail/shared_ptr.hpp>
#include <mutex>
#include <condition_variable>
#include <exception>
#include <stack>

namespace threadsafe {

    struct empty_stack: std::exception{
        const char* what() const throw();
    };

    template <typename T>
    class threadsafe_stack{
    private:
        std::stack<T> data;
        mutable std::mutex m;
    public:
        threadsafe_stack(){}
        threadsafe_stack(const threadsafe_stack& other){
            std::lock_guard<std::mutex> lock(other.m);
            data = other.data;
        }
        threadsafe_stack& operator=(const threadsafe_stack& other) = delete;

        void push(T new_value){
            std::lock_guard<std::mutex> lock(m);
            data.push(std::move(new_value));
        }

        std::shared_ptr<T> pop(){
            std::lock_guard<std::mutex> lock(m);
            if (data.empty()) throw empty_stack();
            std::shared_ptr<T> const res(std::make_shared<T>(std::move(data.top())));
            data.pop();
            return res;
        }

        void pop(T& value){
            std::lock_guard<std::mutex> lock(m);
            if (data.empty()) throw empty_stack();
            value = std::move(data.top());
            data.pop();
        }

        bool empty() const {
            std::lock_guard<std::mutex> lock(m);
            return data.empty();
        }
    };

    template<typename T>
    class threadsafe_queue {
    private:
        struct node {
            std::shared_ptr<T> data;
            std::unique_ptr<node> next;
        };
        std::mutex head_mutex;
        std::unique_ptr<node> head;
        std::mutex tail_mutex;
        node *tail;
        std::condition_variable data_cond;

        node* get_tail() {
            std::lock_guard<std::mutex> tail_lock(tail_mutex);
            return tail;
        }

        std::unique_ptr<node> pop_head() {
            std::unique_ptr<node> old_head = std::move(head);
            head = std::move(old_head->next);
            return old_head;
        }

        std::unique_lock<std::mutex> wait_for_data() {
            std::unique_lock<std::mutex> head_lock(head_mutex);
            data_cond.wait(head_lock, [&] { return head.get() != get_tail(); });
            return std::move(head_lock);
        }

        std::unique_ptr<node> wait_pop_head() {
            std::unique_lock<std::mutex> head_lock(wait_for_data());
            return pop_head();
        }

        std::unique_ptr<node> wait_pop_head(T &value) {
            std::unique_lock<std::mutex> head_lock(wait_for_data());
            value = std::move(*head->data);
            return pop_head();
        }



        std::unique_ptr<node> try_pop_head() {
            std::lock_guard<std::mutex> head_lock(head_mutex);
            if (head.get() == get_tail()) {
                return std::unique_ptr<node>();
            }
            return pop_head();
        }

        std::unique_ptr<node> try_pop_head(T &value) {
            std::lock_guard<std::mutex> head_lock(head_mutex);
            if (head.get() == get_tail()) {
                return std::unique_ptr<node>();
            }
            value = std::move(*head->data);
            return pop_head();
        }

    public:
        threadsafe_queue() : head(new node), tail(head.get()) {}
        threadsafe_queue(const threadsafe_queue &other) = delete;
        threadsafe_queue &operator=(const threadsafe_queue &other) = delete;

        std::shared_ptr<T> try_pop() {
            std::unique_ptr<node> old_head = try_pop_head();
            return old_head ? old_head->data : std::shared_ptr<T>();
        }

        bool try_pop(T &value) {
            std::unique_ptr<node> const old_head = try_pop_head(value);
            //return old_head;
            return true;
        }

        std::shared_ptr<T> wait_and_pop(){
            std::unique_ptr<node> const old_head = wait_pop_head();
            return old_head->data;
        }

        void wait_and_pop(T &value){
            std::unique_ptr<node> const old_head = wait_pop_head(value);
        }

        void push(T new_value);

        bool empty() {
            std::lock_guard<std::mutex> head_lock(head_mutex);
            return (head.get() == get_tail());
        }
    };

    template<typename T>
    void threadsafe_queue<T>::push(T new_value) {
        std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));

        std::unique_ptr<node> p(new node);
        {
            std::lock_guard<std::mutex> tail_lock(tail_mutex);
            tail->data = new_data;
            node *const new_tail = p.get();
            tail->next = std::move(p);
            tail = new_tail;
        }
        data_cond.notify_one();
    }

}
