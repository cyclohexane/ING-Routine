出处：https://www.cnblogs.com/tinkbell/p/3171303.html

 function Person(name, age){
        this.name = name;
        this.age = age;
    }
    Person.prototype.getName = function(){
        alert(this.name);
    }
    Person.prototype.getAge = function(){
        alert(this.age);
    }
    var obj = new Person();
    alert(obj.constructor == Person);// true
    此种方式定义的prototype, constructor是隐藏的, 默认指向Person
    
例子2：
    function Person(name, age){
        this.name = name;
        this.age = age;
    }
    Person.prototype = {
        getName: function(){
           alert(this.name);
        },
        getAge: function(){
           alert(this.age);
        }
    }
    var obj = new Person();
    alert(obj.constructor == Person);// false
    
    为什么是false? 这种定义prototype, 是把prototype重写了, 覆盖了默认的constructor。
    换句话说, 其实这种方式就是给属性重新赋值了, 所以导致默认的constructor被覆盖。
    此时的obj.constructor将指向的是Object。
    
    改写一下上面的：
    Person.prototype = {
        constructor: Person, //强制指向Person
        getName: function(){
           alert(this.name);
        },
        getAge: function(){
           alert(this.age);
        }
    }
    此时constructor就指向Person了。