DOM全称？
> Document Object Model。

DOM作用？
> 将网页转为一个JS对象从而进行各种操作。

DOM是什么？是JS语法的一部分吗？
> 一个可以用各种语言实现的接口规范。不是JS语法的一部分。

DOM的最小组成单位是？
> 节点（node）。

七种节点类型是？
> Element、Attribute、Text、Comment、Document、DocumentType、DocumentFragment

Node是？
> 浏览器提供一个原生的节点对象Node，七种节点都继承了Node，因此具有一些共同的属性和方法。

哪个原生节点代表整个文档？
> document

DOM树的根节点是？
> `<html>`

节点间的三种层级关系？（除了根节点）
> 父节点（parentNode）、子节点（childNodes）、同级节点关系（sibling）

各节点的类型值及其对应常量？
> - 元素节点：1，对应常量Node.ELEMENT_NODE
> - 属性节点：2，对应常量Node.ATTRIBUTE_NODE
> - 文本节点：3，对应常量Node.TEXT_NODE
> - 注释节点：8，对应常量Node.COMMENT_NODE
> - 文档节点：9，对应常量Node.DOCUMENT_NODE
> - 文档类型节点：10，对应常量Node.DOCUMENT_TYPE_NODE
> - 文档片断节点：11，对应常量Node.DOCUMENT_FRAGMENT_NODE

各节点的名称？
> - 元素节点：大写的标签名（如DIV）
> - 属性节点：属性的名称
> - 文本节点：#text
> - 注释节点：#comment
> - 文档节点：#document
> - 文档类型节点：文档的类型
> - 文档片断节点：#document-fragment

Node节点各属性的含义？
> - nodeType
> 节点的类型（一个整数值）。
> - nodeName
> 节点的名称。
> - nodeValue 
> 当前节点的文本值。只有文本节点和注释节点的nodeValue可以返回结果（也可以赋值）。
> - textContent
> 当前节点及其后代节点的文本值。如果赋值，会用一个（转义后的）新文本节点替换所有原来的子节点。
> - baseURI
> 当前网页的绝对路径。由window.location或`<base>`决定。
> - ownerDocument
> 返回当前节点所在的顶层文档对象，即document对象。
> - nextSibling / previousSibling
> 返回紧跟在当前节点后/前面的第一个同级节点（含文本节点、注释节点）。
> - parentNode
> 返回当前节点的父节点（文档节点、文档片段节点、元素节点）。
> - parentElement
> 返回当前节点的父**元素**节点。
> - firstChild / lastChild
> 返回当前节点的第一个/最后一个子节点（含文本节点、注释节点）。
> - childNodes
> 以NodeList的形式返回子节点（含文本节点、注释节点）。如果当前节点不包括任何子节点，则返回一个空的NodeList集合。由于NodeList对象是一个动态集合，一旦子节点发生变化，立刻会反映在返回结果之中。
> - isConnected
> 新生成的节点（document.createElement）插入文档之前返回false，插入之后返回true。

- appendChild()
> 将一个节点对象作为最后一个子节点插入当前节点。返回值为该子节点。如果已存在于文档中则会移到新位置。
- hasChildNodes()
> 判断当前节点是否有子节点（含文本节点、注释节点）。

判断一个节点有没有子节点的多种方法：
> node.hasChildNodes()
> node.firstChild !== null
> node.childNodes && node.childNodes.length > 0