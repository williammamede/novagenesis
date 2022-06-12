import { Component, OnInit } from '@angular/core';
import { GraphAppService } from './graph-app.service';

@Component({
  selector: 'app-graph-app',
  templateUrl: './graph-app.component.html',
  providers: [GraphAppService],
  styleUrls: ['./graph-app.component.css']
})
export class GraphAppComponent implements OnInit {

  constructor(private graphAppService: GraphAppService) { }

  ngOnInit(): void {
    // get binding each 5 seconds
    setInterval(() => {
      this.graphAppService.showData();
    }, 5000);
  }

  getNgBinding() {
    this.graphAppService.showData();
  }

}
