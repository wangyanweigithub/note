 

光学字符识别（OCR）

玛丽娜·塞缪尔（Marina Samuel）

介绍

如果您的计算机可以洗碗，洗衣服，为您做饭并打扫家里怎么办？我想可以肯定地说，大多数人会很乐意得
到帮助！但是，计算机要以与人类相同的方式执行这些任务，又需要做什么呢？

著名的计算机科学家艾伦·图灵（Alan Turing）提出了图灵测试（Turing Test），该方法可以识别机器是
否具有与人类无法区分的智能。该测试涉及一个人向两个隐藏的实体（一个人，另一个人，一台机器）提出
问题，并试图确定哪个是哪个。如果询问器无法识别该机器，则认为该机器具有人文智能。

尽管围绕图灵测试是否是对智能的有效评估以及我们是否可以构建这样的智能机器存在很多争议，但是毫无
疑问，已经有了某种程度的智能的机器。当前有软件可以帮助机器人导航办公室并执行小任务，或帮助患有
阿尔茨海默氏症的人。人工智能（AI）的更常见示例是Google在搜索某些关键字时估算所需内容的方式，或
者Facebook决定在新闻源中放置内容的方式。

AI的一种众所周知的应用是光学字符识别（OCR）。OCR系统是一种软件，可以将手写字符的图像作为输入并
将其解释为机器可读的文本。将手写支票存入银行机时，您可能不会三思而后行，但在后台进行了一些有趣
的工作。本章将研究一个简单的OCR系统的工作示例，该系统使用人工神经网络（ANN）识别数字。但是首先
，让我们建立更多的上下文。

什么是人工智能？

 虽然图灵对智力的定义听起来很合理，但归根结底，从根本上讲，构成智力的问题仍是哲学上的争论。但
是，计算机科学家已经将某些类型的系统和算法分类为AI的分支。每个分支用于解决某些问题。这些分支包
括以下示例以及许多其他示例：

  • 基于世界的一些预定义知识的逻辑和概率推论和推断。例如，模糊推理可帮助恒温器在检测到温度高且
    大气潮湿时决定何时打开空调
  • 启发式搜索。例如，通过搜索所有可能的动作并选择最能改善您的位置的动作，可以使用搜索来找到棋
    类游戏中最佳的下一动作
  • 带有反馈模型的机器学习（ML）。例如OCR之类的模式识别问题。

通常，机器学习涉及使用大型数据集来训练系统以识别模式。可以标记训练数据集，这意味着为给定输入指
定了系统的预期输出，或者未标记意味着未指定预期输出。算法，列车系统与未标记的数据被称为无监督算
法和那些列车用标记的数据被称为监督。存在许多用于创建OCR系统的ML算法和技术，其中ANN是一种方法。

人工神经网络

什么是人工神经网络？

 ANN是由相互通信的互连节点组成的结构。该结构及其功能受到生物大脑中发现的神经网络的启发。
Hebbian理论解释了这些网络如何通过物理改变其结构和链接强度来学习识别模式。类似地，典型的ANN（如
图15.1所示）在节点之间具有连接，该连接的权重随网络学习而更新。标记为“ +1”的节点称为“ 偏差”。节
点的最左边的蓝色列是输入节点，中间的列包含隐藏的节点，最右边的列包含输出节点。可能有许多隐藏节
点列，称为隐藏层。

图15.1-人工神经网络

图15.1-人工神经网络

图15.1中所有圆形节点内部的值表示该节点的输出。如果我们调用ññ从顶层开始的第th个节点 大号大号作
为一个 Ñ （大号）ñ（大号）和之间的联系 一世一世第层节点 大号大号和 ĴĴ第层节点 L + 1大号+1个如 
w（大号）Ĵ一世wĴ（大号）一世，然后是节点的输出 一个（2 ）2一个2（2）是：

一个（2 ）2= f（w（1 ）21X1个+w（1 ）22X2+b（1 ）2）一个2（2）=F（w21（1个）X1个+w22（1个）
X2+b2（1个））

哪里 F（。）F（。）被称为激活功能，bb是偏见。激活功能是节点具有哪种输出类型的决策者。偏置是具
有固定输出1的附加节点，可以将其添加到ANN以提高其准确性。我们将在设计前馈ANN（
neural_network_design.py）中看到有关这两个方面的更多详细信息。

这种类型的网络拓扑称为前馈神经网络，因为网络中没有循环。节点输出被输入的神经网络被称为递归神经
网络。有许多算法可用于训练前馈ANN。一种常用的算法称为反向传播。我们将在本章中实现的OCR系统将使
用反向传播。

我们如何使用人工神经网络？

像大多数其他机器学习方法一样，使用反向传播的第一步是决定如何将我们的问题转换或简化为可以由ANN
解决的问题。换句话说，我们如何处理输入数据，以便将其输入到ANN中？对于我们的OCR系统，我们可以使
用给定数字的像素位置作为输入。值得注意的是，通常情况下，选择输入数据格式并不是那么简单。例如，
如果我们正在分析大图像以识别其中的形状，则可能需要对图像进行预处理以识别其中的轮廓。这些轮廓将
作为输入。

一旦确定了输入数据格式，下一步将是什么？由于反向传播是一种监督算法，因此需要使用标记数据对其进
行训练，如什么是人工智能？。因此，当通过像素位置作为训练输入时，我们还必须传递相关的数字。这意
味着我们必须找到或收集大量的绘制数字和相关值的数据集。

下一步是将数据集划分为训练集和验证集。训练数据用于运行反向传播算法以设置ANN的权重。验证数据用
于使用经过训练的网络进行预测并计算其准确性。如果我们在数据上比较反向传播与另一种算法的性能，则
将数据分成50％用于训练，25％用于比较两种算法（验证集）的性能以及最后25％用于测试算法的准确性选
择的算法（测试集）。由于我们没有比较算法，因此我们可以将25％的数据集之一作为训练数据集的一部分
，并使用75％的数据来训练网络，并使用25％的数据来验证网络是否训练得当。

识别人工神经网络的准确性的目的是双重的。首先，是要避免过度拟合的问题。当网络在预测训练集上的准
确性比验证集高得多时，就会发生过度拟合。过度拟合告诉我们，所选训练数据的概括性不够好，需要完善
。其次，测试几种不同数量的隐藏层和隐藏节点的准确性有助于设计最佳的ANN大小。最佳的ANN大小将具有
足够的隐藏节点和层以进行准确的预测，但节点/连接的数量也应尽可能少，以减少可能减慢训练和预测的
计算开销。一旦确定了最佳大小，并且对网络进行了培训，就可以进行预测了！

简单OCR系统中的设计决策

 在最后几段中，我们介绍了前馈ANN的一些基础知识以及如何使用它们。现在该讨论我们如何构建OCR系统
。

首先，我们必须决定我们希望我们的系统能够做什么。为简单起见，让我们允许用户绘制单个数字并能够使
用该绘制数字训练OCR系统，或请求系统预测绘制数字是什么。尽管OCR系统可以在单台计算机上本地运行，
但是通过设置客户端服务器可以提供更大的灵活性。它使对ANN的众包培训成为可能，并允许功能强大的服
务器处理密集的计算。

我们的OCR系统将由5个主要组件组成，分为5个文件。将有：

  • 客户（ocr.js）
  • 伺服器（server.py）
  • 简单的用户界面（ocr.html）
  • 经过反向传播训练的ANN（ocr.py）
  • ANN设计脚本（neural_network_design.py）

用户界面将很简单：用于在其上绘制数字的画布和用于训练ANN或请求预测的按钮。客户端将收集绘制的数
字，将其转换为数组，然后将其传递给服务器以作为训练样本或预测请求进行处理。服务器将通过对ANN模
块进行API调用来简单地路由训练或预测请求。ANN模块将在首次初始化时使用现有数据集训练网络。然后它
将ANN权重保存到文件中，并在以后的启动中重新加载它们。该模块是训练和预测逻辑的核心所在。最后，
设计脚本用于试验不同的隐藏节点数并确定最有效的方法。总之，这些部分为我们提供了一个非常简单但实
用的OCR系统。

现在，我们已经考虑了系统如何在更高层次上运行，现在是将概念放入代码中的时候了！

简单界面（ocr.html）

如前所述，第一步是收集数据以训练网络。我们可以将一系列手写数字上载到服务器，但这很尴尬。相反，
我们可以让用户使用HTML画布实际在页面上手写数字。然后，我们可以给他们一些选择来训练或测试网络，
其中训练网络还涉及指定绘制什么数字。这样，通过使人们指向网站以接收他们的输入，可以轻松地将数据
收集外包。这是一些HTML入门。

<html>
<head>
    <script src="ocr.js"></script>
    <link rel="stylesheet" type="text/css" href="ocr.css">
</head>
<body onload="ocrDemo.onLoadFunction()">
    <div id="main-container" style="text-align: center;">
        <h1>OCR Demo</h1>
        <canvas id="canvas" width="200" height="200"></canvas>
        <form name="input">
            <p>Digit: <input id="digit" type="text"> </p>
            <input type="button" value="Train" onclick="ocrDemo.train()">
            <input type="button" value="Test" onclick="ocrDemo.test()">
            <input type="button" value="Reset" onclick="ocrDemo.resetCanvas();"/>
        </form>
    </div>
</body>
</html>

OCR客户（ocr.js）

由于可能很难看清HTML画布上的单个像素，因此我们可以将ANN输入的单个像素表示为10x10实像素的平方。
因此，实际的画布为200x200像素，从ANN的角度来看，它由20x20的画布表示。以下变量将帮助我们跟踪这
些测量。

var ocrDemo = {
    CANVAS_WIDTH: 200,
    TRANSLATED_WIDTH: 20,
    PIXEL_WIDTH: 10, // TRANSLATED_WIDTH = CANVAS_WIDTH / PIXEL_WIDTH

然后，我们可以在新的表示形式中勾勒出像素的轮廓，以便于查看。在这里，我们有一个由生成的蓝色网格
drawGrid()。

    drawGrid: function(ctx) {
        for (var x = this.PIXEL_WIDTH, y = this.PIXEL_WIDTH;
                 x < this.CANVAS_WIDTH; x += this.PIXEL_WIDTH,
                 y += this.PIXEL_WIDTH) {
            ctx.strokeStyle = this.BLUE;
            ctx.beginPath();
            ctx.moveTo(x, 0);
            ctx.lineTo(x, this.CANVAS_WIDTH);
            ctx.stroke();

            ctx.beginPath();
            ctx.moveTo(0, y);
            ctx.lineTo(this.CANVAS_WIDTH, y);
            ctx.stroke();
        }
    },

我们还需要以一种可以发送到服务器的形式存储在网格上绘制的数据。为简单起见，我们可以使用称为的数
组data，将无色的黑色像素标记为0，将有色的白色像素标记为1。我们还在画布上需要一些鼠标侦听器，因
此我们知道何时fillSquare()在用户绘制数字时调用将像素着色为白色。这些侦听器应跟踪我们是否处于绘
制状态，然后调用fillSquare()以进行一些简单的数学运算并确定需要填充哪些像素。

    onMouseMove: function(e, ctx, canvas) {
        if (!canvas.isDrawing) {
            return;
        }
        this.fillSquare(ctx,
            e.clientX - canvas.offsetLeft, e.clientY - canvas.offsetTop);
    },

    onMouseDown: function(e, ctx, canvas) {
        canvas.isDrawing = true;
        this.fillSquare(ctx,
            e.clientX - canvas.offsetLeft, e.clientY - canvas.offsetTop);
    },

    onMouseUp: function(e) {
        canvas.isDrawing = false;
    },

    fillSquare: function(ctx, x, y) {
        var xPixel = Math.floor(x / this.PIXEL_WIDTH);
        var yPixel = Math.floor(y / this.PIXEL_WIDTH);
        this.data[((xPixel - 1)  * this.TRANSLATED_WIDTH + yPixel) - 1] = 1;

        ctx.fillStyle = '#ffffff';
        ctx.fillRect(xPixel * this.PIXEL_WIDTH, yPixel * this.PIXEL_WIDTH,
            this.PIXEL_WIDTH, this.PIXEL_WIDTH);
    },

现在我们正在接近多汁的东西！我们需要一个准备训练数据以发送到服务器的功能。这里我们有一个相对简
单的train()函数，该函数对要发送的数据进行一些错误检查，trainArray然后将其添加到并通过调用将其
发送出去sendData()。

    train: function() {
        var digitVal = document.getElementById("digit").value;
        if (!digitVal || this.data.indexOf(1) < 0) {
            alert("Please type and draw a digit value in order to train the network");
            return;
        }
        this.trainArray.push({"y0": this.data, "label": parseInt(digitVal)});
        this.trainingRequestCount++;

        // Time to send a training batch to the server.
        if (this.trainingRequestCount == this.BATCH_SIZE) {
            alert("Sending training data to server...");
            var json = {
                trainArray: this.trainArray,
                train: true
            };

            this.sendData(json);
            this.trainingRequestCount = 0;
            this.trainArray = [];
        }
    },

一个有趣的设计，这里值得一提的是利用trainingRequestCount，trainArray和BATCH_SIZE。这里发生的事
情BATCH_SIZE是一个预定义的常量，该常量用于在客户端向服务器发送要由OCR处理的批处理请求之前，客
户端将跟踪多少培训数据。批处理请求的主要原因是避免一次处理许多请求而使服务器不堪重负。如果存在
许多客户端（例如，许多用户正在ocr.html培训系统的页面上），或者如果客户端中存在另一层，该层接受
扫描的绘制数字并将其转换为像素以训练网络，则BATCH_SIZE1会导致许多不必要的请求。这种方法很好，
因为它为客户端提供了更大的灵活性，但是，实际上，批处理还应在需要时在服务器上进行。可能发生拒绝
服务（DoS）攻击，其中恶意客户端故意向服务器发送许多请求以使其不堪重负，从而使其崩溃。

我们还将需要一个test()功能。与相似train()，它应该对数据的有效性进行简单检查并将其发送出去。对
于test()，但是，没有配料发生，因为用户应该能够要求的预测，并得到立竿见影的效果。

    test: function() {
        if (this.data.indexOf(1) < 0) {
            alert("Please draw a digit in order to test the network");
            return;
        }
        var json = {
            image: this.data,
            predict: true
        };
        this.sendData(json);
    },

最后，我们将需要一些函数来发出HTTP POST请求，接收响应以及处理过程中的任何潜在错误。

    receiveResponse: function(xmlHttp) {
        if (xmlHttp.status != 200) {
            alert("Server returned status " + xmlHttp.status);
            return;
        }
        var responseJSON = JSON.parse(xmlHttp.responseText);
        if (xmlHttp.responseText && responseJSON.type == "test") {
            alert("The neural network predicts you wrote a \'"
                   + responseJSON.result + '\'');
        }
    },

    onError: function(e) {
        alert("Error occurred while connecting to server: " + e.target.statusText);
    },

    sendData: function(json) {
        var xmlHttp = new XMLHttpRequest();
        xmlHttp.open('POST', this.HOST + ":" + this.PORT, false);
        xmlHttp.onload = function() { this.receiveResponse(xmlHttp); }.bind(this);
        xmlHttp.onerror = function() { this.onError(xmlHttp) }.bind(this);
        var msg = JSON.stringify(json);
        xmlHttp.setRequestHeader('Content-length', msg.length);
        xmlHttp.setRequestHeader("Connection", "close");
        xmlHttp.send(msg);
    }

伺服器（server.py）

尽管这是一台仅用于中继信息的小型服务器，但我们仍然需要考虑如何接收和处理HTTP请求。首先，我们需
要确定要使用哪种HTTP请求。在上一节中，客户端正在使用POST，但是为什么要对此做出决定？由于数据正
在发送到服务器，因此PUT或POST请求最有意义。我们只需要发送一个json正文，而没有URL参数。因此，从
理论上讲，GET请求也可以工作，但从语义上讲是没有意义的。但是，PUT和POST之间的选择是程序员之间长
期存在的争论。KNPLabs 幽默地总结了这些问题。

另一个要考虑的是，是否发送“火车”与“预测”的要求，以不同的端点（例如http://localhost/train与
http://localhost/predict）或同一个端点，然后分别处理数据。在这种情况下，我们可以采用后一种方法
，因为在每种情况下，数据处理之间的差异很小，足以适合使用简短的if语句。实际上，如果服务器要对每
种请求类型进行更详细的处理，最好将它们作为单独的端点。这一决定反过来影响了何时使用哪种服务器错
误代码。例如，当在有效负载中未指定“火车”或“预测”时，将发送400“错误请求”错误。如果改为使用单独
的端点，则不会有问题。OCR系统在后台执行的处理可能由于任何原因而失败，如果在服务器内未正确处理
，则会发送500“内部服务器错误”。再次，如果端点是分开的，将有更多的空间来详细发送更合适的错误。
例如，确定内部服务器错误实际上是由错误的请求引起的。

最后，我们需要确定何时以及在何处初始化OCR系统。一个好的方法是在server.py但是在服务器启动之前。
这是因为在首次运行时，OCR系统需要在首次启动时就一些预先存在的数据训练网络，这可能需要几分钟。
如果服务器在此处理完成之前启动，则在给定当前实现的情况下，由于尚未初始化OCR对象，因此任何训练
或预测请求都将引发异常。另一种可能的实现方式可能是创建一些不准确的初始ANN，以用于前几个查询，
而新的ANN在后台进行异步训练。这种替代方法的确允许立即使用ANN，但是实现更为复杂，并且只有在服务
器重置后才能节省启动时间。对于需要高可用性的OCR服务，这种类型的实现将更为有益。

在这里，我们将大多数服务器代码包含在一个处理POST请求的简短函数中。

    def do_POST(s):
        response_code = 200
        response = ""
        var_len = int(s.headers.get('Content-Length'))
        content = s.rfile.read(var_len);
        payload = json.loads(content);

        if payload.get('train'):
            nn.train(payload['trainArray'])
            nn.save()
        elif payload.get('predict'):
            try:
                response = {
                    "type":"test",
                    "result":nn.predict(str(payload['image']))
                }
            except:
                response_code = 500
        else:
            response_code = 400

        s.send_response(response_code)
        s.send_header("Content-type", "application/json")
        s.send_header("Access-Control-Allow-Origin", "*")
        s.end_headers()
        if response:
            s.wfile.write(json.dumps(response))
        return

设计前馈ANN（neural_network_design.py）

 在设计前馈ANN时，必须考虑一些因素。首先是要使用的激活功能。我们之前提到过激活功能是节点输出的
决策者。激活功能做出的决定类型将帮助我们决定使用哪个决定。在我们的情况下，我们将设计一个ANN，
为每个数字（0-9）输出一个介于0和1之间的值。接近1的值表示ANN预测这是绘制的数字，接近0的值表示其
预测不是绘制的数字。因此，我们需要一个激活函数，使其输出接近于0或接近于1。我们还需要一个可微的
函数，因为在反向传播计算中我们需要导数。在这种情况下，常用的函数是S型，因为它同时满足这两个约
束。 常见激活函数及其属性的不错的列表。

要考虑的第二个因素是我们是否要包括偏见。我们之前已经提到过几次偏见，但是并没有真正谈论过它们是
什么或我们为什么使用它们。让我们尝试回到图15.1中如何计算节点的输出来理解这一点。假设我们有一个
输入节点和一个输出节点，我们的输出公式为ÿ= f（宽x ）ÿ=F（wX），在哪里 ÿÿ 是输出， F（）F（）是
激活功能， ww 是节点之间链接的权重，并且 XX是节点的变量输入。偏置本质上是一个节点，其输出始终
为1个1个。这会将输出公式更改为ÿ= f（w x + b ）ÿ=F（wX+b）哪里 bb是偏置节点和下一个节点之间的连
接权重。如果我们考虑ww 和 bb 作为常数和 XX 作为变量，然后加上一个偏差，将常数添加到我们的线性
函数输入中 F（。）F（。）。

因此，加上偏见可以使 ÿÿ-intercept，通常为节点的输出提供更大的灵活性。包括偏差通常是个好习惯，
尤其是对于输入和输出数量较少的ANN。偏移使ANN的输出具有更大的灵活性，从而为ANN提供了更多的准确
性空间。没有偏见，我们使用人工神经网络做出正确预测的可能性就较小，或者需要更多隐藏节点才能做出
更准确的预测。

其他要考虑的因素是隐藏层数和每层隐藏节点数。对于具有许多输入和输出的较大的人工神经网络，这些数
字是通过尝试不同的值并测试网络性能来确定的。在这种情况下，通过训练给定大小的ANN并查看正确分类
的验证集的百分比来衡量性能。在大多数情况下，单个隐藏层足以获得不错的性能，因此我们仅在此处尝试
隐藏节点的数量。

# Try various number of hidden nodes and see what performs best
for i in xrange(5, 50, 5):
    nn = OCRNeuralNetwork(i, data_matrix, data_labels, train_indices, False)
    performance = str(test(data_matrix, data_labels, test_indices, nn))
    print "{i} Hidden Nodes: {val}".format(i=i, val=performance)

在这里，我们以5到5的增量在5到50个隐藏节点之间初始化ANN，然后调用该test()函数。

def test(data_matrix, data_labels, test_indices, nn):
    avg_sum = 0
    for j in xrange(100):
        correct_guess_count = 0
        for i in test_indices:
            test = data_matrix[i]
            prediction = nn.predict(test)
            if data_labels[i] == prediction:
                correct_guess_count += 1

        avg_sum += (correct_guess_count / float(len(test_indices)))
    return avg_sum / 100

内循环计算正确分类的数量，然后将其除以最后尝试分类的数量。这给出了人工神经网络的比率或百分比精
度。由于每次训练ANN时，其权重可能会略有不同，因此我们在外循环中重复此过程100次，因此我们可以对
该特定ANN配置的准确性进行平均。在我们的例子中，示例运行neural_network_design.py如下所示：

PERFORMANCE
-----------
5 Hidden Nodes: 0.7792
10 Hidden Nodes: 0.8704
15 Hidden Nodes: 0.8808
20 Hidden Nodes: 0.8864
25 Hidden Nodes: 0.8808
30 Hidden Nodes: 0.888
35 Hidden Nodes: 0.8904
40 Hidden Nodes: 0.8896
45 Hidden Nodes: 0.8928

从此输出中，我们可以得出结论，认为15个隐藏节点是最佳的。从10到15增加5个节点可以使精度提高1％，
而将精度提高1％则需要再增加20个节点。增加隐藏节点数也会增加计算开销。因此，具有更多隐藏节点的
网络将需要更长的时间进行训练和做出预测。因此，我们选择使用最后一个隐藏的节点数，这会导致准确性
显着提高。当然，在设计ANN时，可能不会出现计算开销的问题，并且拥有最准确的ANN就是当务之急。在这
种情况下，最好选择45个隐藏节点而不是15个。

核心OCR功能

在本节中，我们将讨论如何通过反向传播进行实际训练，如何使用网络进行预测以及针对核心功能的其他关
键设计决策。

通过反向传播进行训练（ocr.py）

我们使用反向传播算法来训练我们的人工神经网络。它由4个主要步骤组成，对训练集中的每个样本重复此
步骤，每次都更新ANN权重。

首先，我们将权重初始化为较小的（介于-1和1之间）随机值。在我们的例子中，我们将它们初始化为值
-0.06和0.06之间，并将它们存储在矩阵theta1，theta2，input_layer_bias，和hidden_layer_bias。由于
一层中的每个节点都链接到下一层中的每个节点，因此我们可以创建一个矩阵，该矩阵具有m行和n列，其中
n是一层中的节点数，m是相邻层中的节点数。该矩阵将代表这两层之间链接的所有权重。在这里，theta1有
400列用于20x20像素的输入和num_hidden_nodes行。同样，theta2表示隐藏层和输出层之间的链接。它具有
num_hidden_nodes列和NUM_DIGITS（10）行。其他两个向量（1行），input_layer_bias和
hidden_layer_bias 代表偏见。

    def _rand_initialize_weights(self, size_in, size_out):
        return [((x * 0.12) - 0.06) for x in np.random.rand(size_out, size_in)]

            self.theta1 = self._rand_initialize_weights(400, num_hidden_nodes)
            self.theta2 = self._rand_initialize_weights(num_hidden_nodes, 10)
            self.input_layer_bias = self._rand_initialize_weights(1,
                                                                  num_hidden_nodes)
            self.hidden_layer_bias = self._rand_initialize_weights(1, 10)

第二步是前向传播，这实际上是按照什么是神经网络来计算节点输出。，从输入节点开始逐层。这里y0是一
个大小为400的数组，其中包含我们希望用于训练ANN的输入。我们乘以转置来相乘theta1，y0这样我们就得
到了两个具有大小的矩阵，(num_hidden_nodes x 400) * (400 x 1)并且得到了一个大小为
num_hidden_​​nodes的隐藏层的输出结果向量。然后，我们添加偏差矢量，并将矢量化的S型激活函数应用
于此输出矢量，从而得出y1。y1是我们隐藏层的输出向量。再次重复相同的过程以计算y2输出节点。y2现在
是我们的输出层向量，其值表示其索引为绘制数字的可能性。例如，如果某人绘制8，则y2如果ANN已做出正
确的预测，则第8个索引处的值将最大。但是，6可能比1更可能成为绘制数字，因为它看起来与8更加相似，
并且随着8的出现，更可能用完要绘制的相同像素。y2随着ANN的每增加一个绘制数字，精度就会提高。训练
有素。

    # The sigmoid activation function. Operates on scalars.
    def _sigmoid_scalar(self, z):
        return 1 / (1 + math.e ** -z)

            y1 = np.dot(np.mat(self.theta1), np.mat(data['y0']).T)
            sum1 =  y1 + np.mat(self.input_layer_bias) # Add the bias
            y1 = self.sigmoid(sum1)

            y2 = np.dot(np.array(self.theta2), y1)
            y2 = np.add(y2, self.hidden_layer_bias) # Add the bias
            y2 = self.sigmoid(y2)

第三步是反向传播，这涉及计算输出节点处的错误，然后计算每个中间层的错误，然后再返回到输入。在这
里，我们首先创建一个预期的输出向量，在数字的索引处actual_vals使用a 1来代表绘制数字的值，0否则
使用s。输出节点上的误差向量output_errors，是通过y2从中减去实际输出向量，来计算的actual_vals。
之后，对于每个隐藏层，我们计算两个分量。首先，我们将下一层的转置权重矩阵乘以其输出误差。然后，
我们将激活函数的导数应用于上一层。然后，我们在这两个分量上执行逐元素乘法，从而为隐藏层提供了错
误向量。这里我们称之为hidden_errors。

            actual_vals = [0] * 10
            actual_vals[data['label']] = 1
            output_errors = np.mat(actual_vals).T - np.mat(y2)
            hidden_errors = np.multiply(np.dot(np.mat(self.theta2).T, output_errors),
                                        self.sigmoid_prime(sum1))

权重更新可根据先前计算的误差来调整ANN权重。权重通过矩阵乘法在每一层进行更新。每层的误差矩阵乘
以前一层的输出矩阵。然后将乘积乘以称为学习率的标量，然后将其加到权重矩阵中。学习率是介于0和1之
间的值，它会影响ANN中学习的速度和准确性。学习率值越大，生成的ANN学习越快，但准确性越低；而学习
率值越小，生成的ANN学习越慢，但准确性更高。在我们的案例中，学习率的值相对较小，为0.1。这很好用
，因为我们不需要立即对ANN进行培训即可让用户继续进行培训或预测请求。

            self.theta1 += self.LEARNING_RATE * np.dot(np.mat(hidden_errors),
                                                       np.mat(data['y0']))
            self.theta2 += self.LEARNING_RATE * np.dot(np.mat(output_errors),
                                                       np.mat(y1).T)
            self.hidden_layer_bias += self.LEARNING_RATE * output_errors
            self.input_layer_bias += self.LEARNING_RATE * hidden_errors

测试经过训练的网络（ocr.py）

通过反向传播对ANN进行训练后，使用它进行预测就非常简单了。正如我们在此处看到的，我们首先以y2与
反向传播的第2步相同的方式计算ANN的输出。然后，我们在向量中寻找具有最大值的索引。该索引是ANN预
测的数字。

    def predict(self, test):
        y1 = np.dot(np.mat(self.theta1), np.mat(test).T)
        y1 =  y1 + np.mat(self.input_layer_bias) # Add the bias
        y1 = self.sigmoid(y1)

        y2 = np.dot(np.array(self.theta2), y1)
        y2 = np.add(y2, self.hidden_layer_bias) # Add the bias
        y2 = self.sigmoid(y2)

        results = y2.T.tolist()[0]
        return results.index(max(results))

其他设计决策（ocr.py）

在线提供了许多资源，这些资源更详细地介绍了反向传播的实现。一个很好的资源是威拉米特大学的一门课
程。它遍历了反向传播的步骤，然后解释了如何将其转换为矩阵形式。尽管使用矩阵的计算量与使用循环的
计算量相同，但好处在于，使用更少的嵌套循环，代码更简单易读。如我们所见，整个训练过程是使用矩阵
代数用25行代码编写的。

如在简单OCR系统中的设计决策介绍中所述，保持ANN的权重意味着当服务器关闭或由于任何原因突然关闭时
，我们不会失去训练它的进度。我们通过将权重作为JSON写入文件来保持权重。启动时，OCR将ANN保存的权
重加载到内存中。OCR不会在内部调用保存功能，而是由服务器决定何时执行保存。在我们的例子中，服务
器在每次更新后保存权重。这是一种快速，简单的解决方案，但并不是最佳选择，因为写入磁盘非常耗时。
这也阻止了我们处理多个并发请求，因为没有机制可以防止同时写入同一文件。在更复杂的服务器中，

    def save(self):
        if not self._use_file:
            return

        json_neural_network = {
            "theta1":[np_mat.tolist()[0] for np_mat in self.theta1],
            "theta2":[np_mat.tolist()[0] for np_mat in self.theta2],
            "b1":self.input_layer_bias[0].tolist()[0],
            "b2":self.hidden_layer_bias[0].tolist()[0]
        };
        with open(OCRNeuralNetwork.NN_FILE_PATH,'w') as nnFile:
            json.dump(json_neural_network, nnFile)

    def _load(self):
        if not self._use_file:
            return

        with open(OCRNeuralNetwork.NN_FILE_PATH) as nnFile:
            nn = json.load(nnFile)
        self.theta1 = [np.array(li) for li in nn['theta1']]
        self.theta2 = [np.array(li) for li in nn['theta2']]
        self.input_layer_bias = [np.array(nn['b1'][0])]
        self.hidden_layer_bias = [np.array(nn['b2'][0])]

结论

既然我们已经了解了AI，ANN，反向传播以及构建了端到端OCR系统，那么让我们来回顾一下本章的重点和概
况。

我们从本章开始，介绍了AI，ANN和我们将要实现的大致知识。我们讨论了什么是AI以及如何使用AI的示例
。我们看到，人工智能本质上是一组算法或解决问题的方法，可以以类似于人类的方式为问题提供答案。然
后，我们看了前馈ANN的结构。我们了解到，在给定节点上计算输出就像对先前节点的输出与其连接权重的
乘积求和一样简单。我们讨论了如何通过首先格式化输入并将数据划分为训练集和验证集来使用ANN。

一旦有了一些背景知识，我们就开始谈论创建一个基于Web的客户端-服务器系统，该系统将处理用户培训或
测试OCR的请求。然后，我们讨论了客户端如何将绘制的像素解释为数组，并向OCR服务器执行HTTP请求以执
行训练或测试。我们讨论了我们的简单服务器如何读取请求以及如何通过测试几个隐藏节点数的性能来设计
ANN。最后，我们完成了反向传播的核心培训和测试代码。

尽管我们已经构建了看似功能强大的OCR系统，但本章只是简单地介绍了实际OCR系统如何工作。更复杂的
OCR系统可能具有预处理输入，使用混合ML算法，具有更广泛的设计阶段或其他进一步的优化。

Google 翻译

原文

提供更好的翻译建议
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
